/*
	My actor CPP file
	TODO:
	
*/

#include "Actor.h"
#include "Game\Camera\CamManager.h"
#include "Game\Level\Level.h"
#include "Game\Level\World.h"

// Internal Vars
float fLastCamY = ZERO_FLOAT;


cEntity *pActor = NULL;


void ActorUpdatePhysicHook ( Leadwerks::Entity* e ) { if ( pActor ) pActor->PhysicUpdate(); }
void ActorUpdateOnFrameHook( Leadwerks::Entity* e ) { if ( pActor ) pActor->OnFrame(); }



// Constructor
cActor::cActor( Leadwerks::Vec3 Position, Leadwerks::Vec3 Rotation, Leadwerks::Vec3 Scale ) {

	cMsgDBG( "cActor::Constructor" );

	// Set internal variables
	this->SetPosition( Position );
	this->SetRotation( Rotation );
	this->SetScale( Scale );
	this->iID = Level()->NewID();
	this->sSection = sName = "Actor";
	this->SetType( eENTITY_TYPE_ACTOR );

	cSection *pSection = LTXReader.GetSection( "Actor" );
	if ( !pSection ) {
		cMsgCRT( "cActor::Constructor: \"Actor\" section not found" );
		return; // leave bIsOK = false
	}

	pEntity = Leadwerks::Pivot::Create();
	pEntity->SetMass( pSection->GetFloat( "phMass" ) );
	pEntity->SetPosition( Position );
	pEntity->SetRotation( Rotation );
	pEntity->SetPhysicsMode( Leadwerks::Entity::CharacterPhysics );
	pEntity->SetCollisionType( Leadwerks::Collision::Character );
	pEntity->SetShadowMode( 0 );
	pEntity->SetPickMode( 0 );
	pEntity->SetFriction( 0.0, 0.0 );
	pEntity->Hide();

	// Utils.h
	SetFloat3( fWalkSpeed,		fWalkJumpCoef,		fWalkStamina, 	GetMultiFloat3( pSection, "Walk",	 1,	2, 3 ) );
	SetFloat3( fRunSpeed,		fRunJumpCoef,		fRunStamina,	GetMultiFloat3( pSection, "Run",	 1, 2, 3 ) );
	SetFloat3( fCrouchSpeed,	fCrouchJumpCoef, 	fCrouchStamina,	GetMultiFloat3( pSection, "Crouch",	 1, 2, 3 ) );
	SetFloat2( fJumpForce,		fJumpStamina, GetMultiFloat2( pSection, "Jump", 1, 2 ) );

	fStaminaRestore = pSection->GetFloat( "StaminaRestore", 0.f );
	fStaminaRunMin  = pSection->GetFloat( "StaminaRunMin", 0.3f );
	fStaminaJumpMin = pSection->GetFloat( "StaminaJumpMin", 0.4f );

	// Load Actor parameters
	SetHealth(      pSection->GetFloat( "Health", 100.f )    );
	SetStamina( 1.f );
	SetMaxItemMass( pSection->GetFloat( "MaxItemsMass", 0.0f ) );
	
	if ( !( pFootstepManager = new cFootstepManager( "Actor" ) )->IsOK() ) {
		return; // leave bIsOK = false
	}

	InputManager = cInputManager();
	pFlashLight = new cFlashLight();

	// For hands and weapons
//	weapontag = Leadwerks::Pivot::Create( camera );

	pEntity->AddHook( Leadwerks::Entity::UpdatePhysicsHook,	ActorUpdatePhysicHook );
	pEntity->AddHook( Leadwerks::Entity::UpdateWorldHook,	ActorUpdateOnFrameHook );

	bIsOK = true;
	cMsgDBG( "cActor::Constructor OK" );
}


// Public
cActor::~cActor() {
	SAFE_DELETE( pFootstepManager );
	SAFE_DELETE( pFlashLight );
	SAFE_RELEASE( pHitBox );
	SAFE_RELEASE( pEntity );
	SAFE_RELEASE( pWeaponTag );
	SAFE_DELETE( pActor );
}

// Public
void cActor::PhysicUpdate() {

	// call once per frame
	if ( this->CallbackHasCalled() ) return;
	else this->OnCallbackCall();

	// Varable list
	bool bIsCrouched = IsCrouched();
	bool bIsMoving  = false;
	bool bIsWalking = false;
	bool bIsRunning = false;
	bool bIsJumping = false;
	bool bIsHanging = false;
	bool bIsFalling = false;
	this->fJump		= 0.0; // reset jump force

	// Update inputs
	InputManager.Update();

	bool bCrouchButtonPressed	= InputManager.Crouch();
	bool bJumpButtonPressed		= InputManager.Jump();
	bool bRunButtonHolden		= InputManager.Run();
	float fMove					= InputManager.Forward() - InputManager.Backward();
	float fStrafe				= InputManager.StrafeRight() - InputManager.StrafeLeft();

	// Set rotation from leadwerks rotation to my cEntity rotation
	this->SetRotation( pEntity->GetRotation() );
	pFlashLight->PhysicUpdate();

	// if using ladder
	if ( this->IsClimbing() ) {
		
		float fDirMul = ( ( fMove > 0 ) ? FORWARD_MUL : BACKWARD_MUL );
		fMove *= ( fWalkSpeed * 0.04 ) * fDirMul;

//		pEntity->SetPhysicsMode( Leadwerks::Entity::RigidBodyPhysics );
		pEntity->SetPosition( pEntity->GetPosition() + Leadwerks::Vec3( 0.0, fMove, 0.0 ) );
//		pEntity->charactercontroller->movement = EMPTY_VEC3;
//		pEntity->SetPhysicsMode( Leadwerks::Entity::CharacterPhysics );
//		pEntity->SetInput( 0, 0, 0, 0, false, 0.0, 0.0 );

		fMoveSmooth = fStrafeSmooth = 0.0f;
		fLastCamY = CamManager()->GetCamera()->GetRotation( true ).y;

		this->SetPosition( pEntity->GetPosition() );
		return;
	}


	// Check if is going up or down
	this->SetState( ENTITY_STATES::Hanging, false );
	this->SetState( ENTITY_STATES::Falling, false );
	if ( pEntity->GetAirborne() ) {					// no more on the ground
		if ( pEntity->GetVelocity().y > 0 ) {		// if is going up
			this->SetState( ENTITY_STATES::Hanging, true );
			bIsHanging = true;
		}
		else {										// if falling
			this->SetState( ENTITY_STATES::Falling, true );
			bIsFalling = true;
		}
		this->bLanded = false;
	}

	// check for save cpu ( while airbourne doesn't evaluate inputs )
	if ( ( bIsHanging || bIsFalling ) ) {

		// normally restore stamine while airbourne
		fStamina = L_CLAMP( fStamina + fStaminaRestore, 0.0f, 1.0f );

		// Play step sound when lands over terrain( by checking reached falling speed ( to find a better way!! ) )
		if ( bIsFalling && !bLanded ) { // TODO: play sound and camera animation properly for heavy landing
			Leadwerks::PickInfo pPickInfo = Leadwerks::PickInfo();
			if ( World()->GetWorld()->Pick( vPosition, vPosition - Leadwerks::Vec3( 0.0, 0.05, 0.0 ), pPickInfo, 0.0, false, Leadwerks::Collision::LineOfSight ) ) {
				pFootstepManager->Step( bIsCrouched, pPickInfo );
				fMoveSmooth = fStrafeSmooth = 0.0f;
				bLanded = true;
			}
		}

		// this prevent curve fly orientation
		// by the way simulate last input  but with same vertical orientation
		pEntity->SetInput( fLastCamY, fMoveSmooth, fStrafeSmooth, fJump );
		this->SetPosition( pEntity->GetPosition() );
		return;
	}
	

	// Crouch State
	if ( bCrouchButtonPressed ) {
		if ( bIsCrouched ) {
			if ( !bIsUnderSomething ) {
				CamManager()->GetHeadBobEffect()->Reset( true );
				bIsCrouched = false;
			}
		} else {
			{
				CamManager()->GetHeadBobEffect()->Reset( true );
				bIsCrouched = true;
			}
			if ( bJumpButtonPressed ) fJump = fJumpForce / 2;
		}
	}

	// Jump
	if ( bJumpButtonPressed && !bIsUnderSomething && ( fStamina > fStaminaJumpMin ) ) {
		fStamina -= fJumpStamina;
		fJump += fJumpForce / ( bIsCrouched ? CROUCH_JUMP_BOOST:1 );
		bIsJumping = true;
	}
	
	// Run State
	if ( fStamina <= 0.0 ) bTiredness = true;
	if ( bTiredness && fStamina > fStaminaRunMin ) bTiredness = false;
	if ( bRunButtonHolden && !bTiredness ) {
		if ( bIsCrouched && !bIsUnderSomething ) bIsCrouched = false;
		CamManager()->GetZoomEffect()->Reset();
		bIsRunning = true;
	}

	// process inputs
	if ( ( fMove != 0.0 ) || ( fStrafe != 0.0 ) ) {
		float fDirMul = ( ( fMove > 0 ) ? FORWARD_MUL:BACKWARD_MUL );
		// running back imply go slower than forward
		if ( bIsRunning ) {
			fMove	 *= fRunSpeed *	  fDirMul;
			fStrafe  *= fRunSpeed *   STRAFE_MUL;
			fStamina -= fRunStamina;
		} else if ( bIsCrouched ) {
			fMove	 *= fCrouchSpeed * fDirMul;
			fStrafe  *= fCrouchSpeed * STRAFE_MUL;
			fStamina -= fCrouchStamina;
		} else { // walking
			// halve restore because we are moving any way
			fMove	 *= fWalkSpeed *   fDirMul;
			fStrafe  *= fWalkSpeed *   STRAFE_MUL;
			fStamina += fStaminaRestore / 2;
			bIsWalking = true;
		}

		if ( !bIsJumping ) bIsMoving = true;
	}

	// if don't move regenerates stamina
	if ( ( fMove == 0.0 ) && ( fStrafe == 0.0 ) )
		fStamina += fStaminaRestore;

	// Clamp Stamina between 0.0 and 1.0
	fStamina = Leadwerks::Math::Clamp( fStamina, 0.0f, 1.0f );
	
	// This prevents "speed hack" strafing
	if ( ( fMove != 0.0 ) && ( fStrafe != 0.0 ) ) {
		fMove *= SPEEDHACK_MUL; fStrafe *= SPEEDHACK_MUL;
	}
	
	// boost movements a bit when Jumping
	if ( bIsJumping ) {
		if ( bIsWalking )  { fMove	*= fWalkJumpCoef;	fStrafe	*= fWalkJumpCoef;	fJump	*= fWalkJumpCoef; }
		if ( bIsRunning )  { fMove	*= fRunJumpCoef;	fStrafe	*= fRunJumpCoef;	fJump	*= fRunJumpCoef;  }
		if ( bIsCrouched ) { fJump	*= fCrouchJumpCoef; }
	}

	// Update body movements
	fLastCamY		= CamManager()->GetCamera()->GetRotation( true ).y;
	fMoveSmooth		= Leadwerks::Math::Curve( fMove, fMoveSmooth, 5 );
	fStrafeSmooth	= Leadwerks::Math::Curve( fStrafe, fStrafeSmooth, 5 );
	pEntity->SetInput( fLastCamY, fMoveSmooth, fStrafeSmooth, fJump );

	this->SetPosition( pEntity->GetPosition() );

	// Update all States
	this->SetState( ENTITY_STATES::Crouched, bIsCrouched);
	this->SetState( ENTITY_STATES::Moving,  bIsMoving	);
	this->SetState( ENTITY_STATES::Walking, bIsWalking	);
	this->SetState( ENTITY_STATES::Running, bIsRunning	);
	this->SetState( ENTITY_STATES::Jumping, bIsJumping	);

}

// Public
// Update movements
void cActor::OnFrame( void ) {

	if ( !this->CallbackHasCalled() ) this->PhysicUpdate();
	this->ResetCallback();

	if ( Leadwerks::Window::GetCurrent()->KeyHit( Leadwerks::Key::G ) )
		pEntity->SetGravityMode( pEntity->GetGravityMode() ? false : true );

	float fTimeDelta = Engine()->TimeDelta();

	// Pick eventual collision info until camera
	Leadwerks::Vec3 vCamPos = CamManager()->GetCamera()->GetPosition();
	Leadwerks::Vec3 vCheckHeigth = { 0.f, ( CamManager()->GetStdHeight() / 2 ) * ( fJumpForce / 10 ), 0.f };
	bIsUnderSomething = World()->GetWorld()->Pick( vCamPos, vCamPos + vCheckHeigth, Leadwerks::PickInfo(), 0.36 );
#if _DEBUG 
	Engine()->DrawInfo( "IsIdle %s",	IsIdle() ? "true" : "false" );
	Engine()->DrawInfo( "IsMoving %s",	IsMoving() ? "true" : "false" );
	Engine()->DrawInfo( "IsLeaning %s",	IsLeaning() ? "true" : "false" );
	Engine()->DrawInfo( "IsWalking %s",	IsWalking() ? "true" : "false" );
	Engine()->DrawInfo( "IsRunning %s",	IsRunning() ? "true" : "false" );
	Engine()->DrawInfo( "IsJumping %s",	IsJumping() ? "true" : "false" );

	Engine()->DrawInfo( "IsHanging %s",	IsHanging() ? "true" : "false" );
	Engine()->DrawInfo( "IsFalling %s",	IsFalling() ? "true" : "false" );
	Engine()->DrawInfo( "IsCrouched %s",IsCrouched() ? "true" : "false" );

#endif
	pFlashLight->OnFrame();
	
}
