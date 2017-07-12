/*
	My actor CPP file
	TODO:
	
*/

#include "Actor.h"
#include "Game\Camera\CamManager.h"
#include "Game\Level\Level.h"
#include "Game\Level\World.h"

#define USE_DISTANCE 2.0f
#define USE_RADIUS 0.1f

cEntity *pActor = NULL;


inline void ActorUpdateHook ( /*Leadwerks::Entity* e*/ ) { if ( pActor ) pActor->Update(); }
inline void ActorOnFrameHook( Leadwerks::Entity* e ) { if ( pActor ) pActor->OnFrame(); }



// Constructor
cActor::cActor( Leadwerks::Vec3 Position, Leadwerks::Vec3 Rotation ) {

	cMsgDBG( "cActor::Constructor" );

	////////////////////////////////////////////////////////////////////////////////////////
	// Set internal variables
	{
		this->iID			= Level()->NewID();
		this->sSection		= sName = "Actor";
		this->SetType		( ENTITY::Type::ACTOR );
	}

	cSection *pSection = LTXReader.GetSection( "Actor" );
	if ( !pSection ) {
		cMsgCRT( "cActor::Constructor: \"Actor\" section not found" );
		return; // leave bIsOK = false
	}
	this->SetSection( pSection );

	////////////////////////////////////////////////////////////////////////////////////////
	// Create Leadwerks entity ( pivot )
	{
		pEntity = Leadwerks::Pivot::Create();
		pEntity->SetMass( fPrevMass = pSection->GetFloat( "phMass" ) );
		pEntity->SetPosition( Position );
		pEntity->SetRotation( Rotation );
		pEntity->SetKeyValue( "name", "Actor" );

		// Character Controller
		pEntity->SetPhysicsMode( Leadwerks::Entity::CharacterPhysics );
		pEntity->SetCollisionType( Leadwerks::Collision::Character );
		pEntity->SetFriction( 0.0, 10.0 );
		pEntity->SetBuoyancyMode( false );

		pEntity->SetShadowMode( 0 );
		pEntity->SetPickMode( 0 );
		pEntity->SetMaterial( "Materials\\Effects\\Invisible.mat" );
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Entity Setup
	{
		// Walking
		fWalkSpeed				= pSection->GetMultiValue( "Walk",	1, LTX_READER::VALUE_TYPES::FLOAT );
		fWalkJumpCoef			= pSection->GetMultiValue( "Walk",	2, LTX_READER::VALUE_TYPES::FLOAT );
		fWalkStamina			= pSection->GetMultiValue( "Walk",	3, LTX_READER::VALUE_TYPES::FLOAT );
		
		// Running
		fRunSpeed				= pSection->GetMultiValue( "Run",	1, LTX_READER::VALUE_TYPES::FLOAT );
		fRunJumpCoef			= pSection->GetMultiValue( "Run",	2, LTX_READER::VALUE_TYPES::FLOAT );
		fRunStamina				= pSection->GetMultiValue( "Run",	3, LTX_READER::VALUE_TYPES::FLOAT );
		
		// Crouched
		fCrouchSpeed			= pSection->GetMultiValue( "Crouch",1, LTX_READER::VALUE_TYPES::FLOAT );
		fCrouchJumpCoef			= pSection->GetMultiValue( "Crouch",2, LTX_READER::VALUE_TYPES::FLOAT );
		fCrouchStamina			= pSection->GetMultiValue( "Crouch",3, LTX_READER::VALUE_TYPES::FLOAT );
		
		// Climbing
		fClimbSpeed				= pSection->GetFloat( "Climb", 0.12 );

		// Jumping
		fJumpForce				= pSection->GetMultiValue( "Jump", 1, LTX_READER::VALUE_TYPES::FLOAT );
		fJumpStamina			= pSection->GetMultiValue( "Jump", 2, LTX_READER::VALUE_TYPES::FLOAT );

		// Stamina
		fStaminaRestore			= pSection->GetFloat( "StaminaRestore", 0.f );
		fStaminaRunMin			= pSection->GetFloat( "StaminaRunMin", 0.3f );
		fStaminaJumpMin			= pSection->GetFloat( "StaminaJumpMin", 0.4f );

		// Set Entity parameters
		SetHealth( pSection->GetFloat( "Health", 100.f ) );
		SetStamina( 1.0f );
		SetMaxItemMass( pSection->GetFloat( "MaxItemsMass", 0.0f ) );

	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Footstep manager
	{
		std::string sSection;
		if ( !pSection->bGetString( "FootStepsSection", sSection ) || !( pFootstepManager = new cFootstepManager( sSection ) ) ) {
			return; // leave bIsOK = false
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Flashlight
	pFlashLight = new cFlashLight();
	////////////////////////////////////////////////////////////////////////////////////////
	// Register Update Hook
//	pEntity->AddHook( Leadwerks::Entity::UpdatePhysicsHook, ActorUpdateHook );
	Engine()->RegisterPeriodicHook( 1000 / 60, ActorUpdateHook ); // 60 times per socond, every 16.66666ms

//	if ( !pEntity->GetUserData() ) pEntity->SetUserData( ( cEntity * )this );
	//** Already set in Level.cpp
	////////////////////////////////////////////////////////////////////////////////////////
	// Hold buttons bools
	Engine()->InputManager()->SetHoldCrouch( false );
	Engine()->InputManager()->SetHoldJump( false );
	Engine()->InputManager()->SetHodRun( true );
	////////////////////////////////////////////////////////////////////////////////////////
	// Starting motion type
	this->SetMotionType( LIVE_ENTITY::Motion::Walk );

	bIsOK = true;
	cMsgDBG( "cActor::Constructor OK" );
}


// Public
cActor::~cActor() {

	SAFE_DELETE( pFootstepManager ); 
	SAFE_DELETE( pFlashLight );
	SAFE_RELEASE( pFootsModel );
	SAFE_RELEASE( pHitBox );
	SAFE_RELEASE( pEntity );
///	SAFE_RELEASE( pWeaponTag );
	SAFE_DELETE( pActor );

}

// Private
void cActor::PhysicUpdate_Walk() {

	if ( GetHealth() <= 0.0f ) return;

	cInputManager *pIM			= Engine()->InputManager();
	bool bCrouchButtonPressed	= pIM->Crouch();
	bool bJumpButtonPressed		= pIM->Jump();
	bool bRunButtonHolden		= pIM->Run();
	float fMove					= pIM->Forward() - pIM->Backward();
	float fStrafe				= pIM->StrafeRight() - pIM->StrafeLeft();

	float fFinalJump			= 0.0f;

	// i need this because i use toggle crouch commands
	//	if user want decide by UI:
	//	if ( UI->CrouchToogleOn() )
	if ( pIM->GetHoldCrouch() == false )
		States.bIsCrouched = IsCrouched();

	////////////////////////////////////////////////////////////////////////////////////////
	// Check if is going up or down
	{
		// for save cpu( and for realism ) while airbourne doesn't evaluate inputs
		if ( pEntity->GetAirborne() ) {					// no more on the ground
			( pEntity->GetVelocity().y > 0 ) ? ( States.bIsHanging = true ) : ( States.bIsFalling = true );
		
			// At trasition fron hanging of falling
			if ( PrevStates.bIsHanging && States.bIsFalling )
				// update falling timer ( this excludes time spent while hanging )
				iLastLandTime = Leadwerks::Time::GetCurrent();

			// falling time evaluation ( ms )
			long iFallTime = Leadwerks::Time::GetCurrent() - iLastLandTime;
			if ( iFallTime > 100 )
				bLanded = false;
			if ( iFallTime > 400 )
				bHeavyFall = true;
	//		if ( iFallTime > 600 )
			// need a formula for health subtraction
			// health reduction by too high fall

			// normally restore stamina while airbourne
			fStamina = L_CLAMP( fStamina + fStaminaRestore, 0.0f, 1.0f );

			// Send inputs to LE controller
			// Y axis for orientation is not updated to mantain jump direction
			pEntity->SetInput( fLastCamY, fMoveSmooth, fStrafeSmooth, 0.0f, true );

			return;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Play step sound when lands over terrain
	{
		// TODO: play sound and camera animation properly for heavy landing
		if ( !bLanded ) {	// if not bLanded, actor is airbourne and now has reached terrain
			Leadwerks::PickInfo pPickInfo = Leadwerks::PickInfo();
			Leadwerks::Vec3 vPosition = pEntity->GetPosition();
			if ( World()->GetWorld()->Pick( vPosition, vPosition - Leadwerks::Vec3( 0.0, 1.0, 0.0 ), pPickInfo, 0.5, true /*, Leadwerks::Collision::Projectile*/ ) ) {
				pFootstepManager->Step( pPickInfo, States.bIsCrouched, bHeavyFall );
				CamManager()->GetHeadBobEffect()->Reset( true );
			}
			bHeavyFall = false; // just a reset
			bLanded = true;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Crouch State
	{
		// If crouch button is pressed
		if ( bCrouchButtonPressed ) {

			// If is crouched
			if ( States.bIsCrouched ) {
			
				// if has enough space to stand up
				if ( !bIsUnderSomething ) {
					CamManager()->GetHeadBobEffect()->Reset( true );
					States.bIsCrouched = false;
				}

			} else {
				// When jumping while crouching half jump force is already added
				if ( bJumpButtonPressed ) fFinalJump = fJumpForce / 2.0f;
				CamManager()->GetHeadBobEffect()->Reset( true );
				States.bIsCrouched = true;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Jump
	{
		// If jump button is pressed, has enough space to jump, has stamina requirements and is not dragging an entity
		if ( bJumpButtonPressed && !bIsUnderSomething && ( fStamina > fStaminaJumpMin ) && !IsDragging() ) {

			fStamina -= fJumpStamina;
			fFinalJump += fJumpForce / ( States.bIsCrouched ? CROUCH_JUMP_BOOST : 1.0f );
			fFinalJump *= IsInWater() ? 0.8f : 1.0f;
			States.bIsJumping = true;

		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Run State
	{
		// When stamina reach zero, player became tired
		if ( fStamina <= 0.0 ) bTiredness = true;

		// When player is tired and stamina overcome a defined value player is no more tired
		if ( bTiredness && fStamina > fStaminaRunMin ) bTiredness = false;

		// So if run button is pressed, player is not tired and if crouched can stan up
		if ( bRunButtonHolden && !bTiredness && ( ( States.bIsCrouched && !bIsUnderSomething ) || !States.bIsCrouched ) ) {

			// if is dragging an entity drop that entity
			if ( IsDragging() ) DropEntityDragged();
			CamManager()->GetZoomEffect()->Reset();
			States.bIsCrouched = false;
			States.bIsRunning = true;

		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Process inputs
	// If receive input for moving
	if ( ( fMove != 0.0 ) || ( fStrafe != 0.0 ) ) {

		// moving back imply go slower than forward
		float fDirMul = ( ( fMove > 0 ) ? FORWARD_MUL : BACKWARD_MUL );

		if ( States.bIsRunning ) {
			fMove		*= fRunSpeed *	  fDirMul;
			fStrafe		*= fRunSpeed *   STRAFE_MUL;
			fStamina	-= fRunStamina;

		} else if ( States.bIsCrouched ) {
			fMove		*= fCrouchSpeed * fDirMul;
			fStrafe		*= fCrouchSpeed * STRAFE_MUL;
			fStamina	-= fCrouchStamina;

		} else { // walking
				 // stamina half restored because we are moving, but just walking
			fMove		*= fWalkSpeed *   fDirMul;
			fStrafe		*= fWalkSpeed *   STRAFE_MUL;
			fStamina	+= fStaminaRestore / 2;
			States.bIsWalking = true;
		}

		// Player can jump without moving, so moving is set if is not jumping
		if ( !States.bIsJumping ) States.bIsMoving = true;
	}

	// if don't move stamina regenerates at regular speed
	if ( !fMove && !fStrafe )
		fStamina += fStaminaRestore;

	// This prevents "speed hack" strafing
	if ( fMove && fStrafe ) {
		fMove *= SPEEDHACK_MUL; fStrafe *= SPEEDHACK_MUL;
	}

	// Clamp Stamina between 0.0 and 1.0
	fStamina = Leadwerks::Math::Clamp( fStamina, 0.0f, 1.0f );

	// boost movements when Jumping
	if ( States.bIsJumping ) {
		if ( States.bIsWalking )	{ fMove *= fWalkJumpCoef;	fStrafe *= fWalkJumpCoef;	fFinalJump *= fWalkJumpCoef; }
		if ( States.bIsRunning )	{ fMove *= fRunJumpCoef;	fStrafe *= fRunJumpCoef;	fFinalJump *= fRunJumpCoef; }
		if ( States.bIsCrouched )	{ fFinalJump *= fCrouchJumpCoef; }
	}

	// smooth body movements
	fMoveSmooth		= Leadwerks::Math::Curve( fMove,   fMoveSmooth,	  2 );
	fStrafeSmooth	= Leadwerks::Math::Curve( fStrafe, fStrafeSmooth, 2 );

	// update Y axis for controller orientation
	fLastCamY = CamManager()->GetCamera()->GetRotation( true ).y;

	// Send inputs to LE controller
	pEntity->SetInput( fLastCamY, fMoveSmooth, fStrafeSmooth, fFinalJump, States.bIsCrouched, 1.0f, 0.5f, true );

	// Update internal time value
	// Used for timed operation such as high jump or other things
	iLastLandTime = Leadwerks::Time::GetCurrent();

}

// Private
void cActor::PhysicUpdate_Fly() {

	cInputManager *pIM			= Engine()->InputManager();

	bool bRunButtonHolden		= pIM->Run();

	float fMove					= pIM->Forward() - pIM->Backward();
	float fStrafe				= pIM->StrafeRight() - pIM->StrafeLeft();
	float fVertical				= pIM->Jump() - pIM->Crouch();

	// Modifier
	float fSpeed = ( bRunButtonHolden ? fRunSpeed : fWalkSpeed );

	float fDirMul = ( ( fMove > 0 ) ? FORWARD_MUL : BACKWARD_MUL );
	fMove	*= fSpeed * fDirMul;
	fStrafe *= fSpeed * STRAFE_MUL;
	fVertical *= fSpeed * 0.1;

	fMoveSmooth = fStrafeSmooth = 0.0f;

	// update Y axis for controller orientation
	fLastCamY = CamManager()->GetCamera()->GetRotation( true ).y;

	// Send inputs to LE controller
	pEntity->SetInput( fLastCamY, fMove, fStrafe, 0.0f, false, 0.2, 10.0 );
	pEntity->AddForce( 0.f, fVertical, 0.0f );

	// Stamina regenerates at regular speed
	fStamina += fStaminaRestore;

}

// Private
void cActor::PhysicUpdate_Swim( bool bIsEntityInWater, bool bIsCameraUnderWater, bool bIsCameraReallyUnderWater ) {

	// TODO Manage stamina while in water

	cInputManager *pIM			= Engine()->InputManager();
	float fMove					= pIM->Forward()		- pIM->Backward();
	float fStrafe				= pIM->StrafeRight()	- pIM->StrafeLeft();
	float fUpDown				= pIM->Jump() - pIM->Crouch();
	bool bRunButtonHolden		= pIM->Run();

	// States.bIsMoving can disable HeadMove in favor of HeadBob, that is disabled while swimming
	bool bIsMoving = false;
	if ( ( fMove != 0.0 ) || ( fStrafe != 0.0 ) || ( fUpDown != 0.0 ) ) bIsMoving = true;

	// vertical force is relative to x local axis rotation of camera
	float fVertical = ( CamManager()->GetCamera()->GetRotation().x / 80.f );
	// calculate direction
	fVertical *= -fMove;
	// fMove is relative to vertical direction
	fMove *= 1.f - abs( fVertical );
	// Add inputs from jump - crouch controls
	fVertical += fUpDown;
	// set main speed
	fVertical *= fWalkSpeed;

	float fDirMul = ( ( fMove > 0 ) ? FORWARD_MUL : BACKWARD_MUL );
	fMove		*= ( bRunButtonHolden ? fRunSpeed : fWalkSpeed ) * fDirMul;
	fStrafe		*= ( bRunButtonHolden ? fRunSpeed : fWalkSpeed ) * STRAFE_MUL;

	// This prevents "speed hack" strafing
	if ( fMove && fStrafe ) {
		fMove *= SPEEDHACK_MUL; fStrafe *= SPEEDHACK_MUL;
	}

	fMoveSmooth		= L_CURVE( fMove, fMoveSmooth, 20 );
	fStrafeSmooth	= L_CURVE( fStrafe, fStrafeSmooth, 20 );

	if ( bIsMoving )
		// update Y axis for controller orientation
		fLastCamY = CamManager()->GetCamera()->GetRotation( true ).y;

	// Send inputs to LE controller
	pEntity->SetInput( fLastCamY, fMoveSmooth, fStrafeSmooth, 0.0f, true, 10.f, 200.f );

	Leadwerks::Vec3 v = pEntity->GetVelocity();
	fVerticalSmooth = L_CURVE( fVertical, v.y, bRunButtonHolden ? 10 : 30 );

	if ( !bIsCameraUnderWater )
		fVerticalSmooth = ZERO_FLOAT;

	pEntity->SetVelocity( v.x, fVerticalSmooth, v.z, true );

	// Stamina regenerates at half speed if using sprint otherwise at regular speed
	fStamina += ( bRunButtonHolden ? fStaminaRestore /2.0 : fStaminaRestore );

	// Clamp Stamina between 0.0 and 1.0
	fStamina = Leadwerks::Math::Clamp( fStamina, 0.0f, 1.0f );

}

// Private
void cActor::PhysicUpdate_P1ToP2() {

	cInputManager *pIM				= Engine()->InputManager();
	bool bJumpButtonPressed			= pIM->Jump();
	bool bRunButtonHolden			= pIM->Run();
	float fMove						= pIM->Forward() - pIM->Backward();
	float fStrafe					= pIM->StrafeRight() - pIM->StrafeLeft();

	if ( ( fMove != 0.0 ) || ( fStrafe != 0.0 ) ) States.bIsMoving = true;

	// update internal vars
	fMoveSmooth = fStrafeSmooth = 0.0f;
	fLastCamY = CamManager()->GetCamera()->GetRotation( true ).y;

	// On user input
	if ( fMove != 0.0f ) {
		float fDirMul = ( ( fMove > 0 ) ? FORWARD_MUL : BACKWARD_MUL );
		fMove *= fClimbSpeed * fDirMul;

		float fX_Axis = CamManager()->GetCamera()->GetRotation().x;

		// manually move entity
		Leadwerks::Vec3 vPosition = GetRotationP1ToP2().Normalize() * fMove;

		pEntity->SetPosition( pEntity->GetPosition() + vPosition );
	}

	// Stamina regenerates at regular speed
	fStamina += fStaminaRestore;

	// Clamp Stamina between 0.0 and 1.0
	fStamina = Leadwerks::Math::Clamp( fStamina, 0.0f, 1.0f );

}


// Public
// TODO: falling in water must switch motion to swim, so a control needed for this
void cActor::Update() {

	// trace previuos states
	PrevStates = States;
	
	// Reset "local" states
	States.Reset();

	Leadwerks::Vec3 vCamPos = CamManager()->GetCamera()->GetPosition();

	////////////////////////////////////////////////////////////////////////////////////////
	// Pick eventual collision info from camera to up
	{
		// my check hight formula
		Leadwerks::Vec3 vCheckHeigth = { 0.0f, ( CamManager()->GetStdHeight() / 2 ) * ( fJumpForce / 10 ), 0.0f };
		vCheckHeigth *= IsCrouched() ? 0.5f : 1.0f;
		bIsUnderSomething = World()->GetWorld()->Pick( vCamPos, vCamPos + vCheckHeigth, Leadwerks::PickInfo(), 0.36 );
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Check for usage
	{
		if ( Engine()->InputManager()->Use() ) {
			if ( !IsDragging() ) { // if is not draggind an entity
				Leadwerks::PickInfo pInfo;
				Leadwerks::Vec3 vUsageDistance = Leadwerks::Transform::Point( 0.0f, 0.0f, USE_DISTANCE, CamManager()->GetCamera(), NULL );
				bool bResult = World()->GetWorld()->Pick( vCamPos, vUsageDistance, pInfo, USE_RADIUS, true );
				if ( bResult && pInfo.entity ) {
					Leadwerks::Entity *pEntity = pInfo.entity;

					if ( pEntity->ContainsFunction( "Use" ) )   pEntity->CallFunction( "Use" );
					else
					if ( pEntity->ContainsFunction( "Drag" ) ) { pEntity->CallFunction( "Drag" );
						SetDragging( pEntity );
					}
				}
			}
			else { // if is dragging an entity
				DropEntityDragged();
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Dragging Update
	{
		if ( IsDragging() ) {
			Leadwerks::Vec3 pEntityDraggedPos = Dragged.pEntity->GetPosition( true );
			Leadwerks::Vec3 vNewPosition = Leadwerks::Transform::Point(    Dragged.vPosition, CamManager()->GetCamera(), NULL );
			Leadwerks::Vec3 vNewRotation = Leadwerks::Transform::Rotation( Dragged.vRotation, CamManager()->GetCamera(), NULL );

			float fMaxDifference = 0.5f;
			float fDistance = vNewPosition.DistanceToPoint( pEntityDraggedPos );
			if ( fDistance > 1.5 )  {
				DropEntityDragged();
			}
			else {
				if ( fDistance > fMaxDifference ) {
					vNewPosition = pEntityDraggedPos + ( vNewPosition - pEntityDraggedPos ).Normalize() * fMaxDifference;
					fDistance = fMaxDifference;
				}
				Dragged.pEntity->PhysicsSetPosition( vNewPosition.x, vNewPosition.y, vNewPosition.z, 0.25 * ( IsSwimming() ? 0.5 : 1.0 ) );
				Dragged.pEntity->PhysicsSetRotation( vNewRotation, 0.5 );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Water
	bool bIsEntityInWater, bIsCameraUnderWater, bIsCameraReallyUnderWater;
	if ( !IsClimbing() && World()->GetWorld()->GetWaterMode() ) {
		
		float fWaterHeight		 = World()->GetWorld()->GetWaterHeight();
		// camera is under water level
		bIsCameraUnderWater = ( vCamPos.y - 0.1f ) < fWaterHeight;
		bIsCameraReallyUnderWater = vCamPos.y < fWaterHeight;
		// entity is under water level, but camera is over water level
		bIsEntityInWater = pEntity->GetPosition().y-0.1 < fWaterHeight && !bIsCameraUnderWater;

		SetInWater( bIsEntityInWater );

		// If now camera is over water level, but prev update was under it
		if ( bIsEntityInWater ) {

			// if distance beetwen ground and parent is minus than camera height
			if ( GetAirbourneHeigth() < CamManager()->GetStdHeight() ) {
				// restore walking state
			//	if ( iMotionType != LIVE_ENTITY::Motion::Walk ) {
					SetMotionType( LIVE_ENTITY::Motion::Walk );
				//	SetCrouched( true );
			//	}
			}

		}

		// If camera go under water level enable underwater state
		if ( bIsCameraUnderWater && iMotionType != LIVE_ENTITY::Motion::Swim ) {
			SetSwimming();
		}

		// if actual motion is 'Swim' but is not entity and camera underwater restore 'walk' motion
		if ( iMotionType == LIVE_ENTITY::Motion::Swim && !bIsEntityInWater && !bIsCameraUnderWater )
			SetMotionType( LIVE_ENTITY::Motion::Walk );

		if ( bIsCameraReallyUnderWater ) {
			SetUnderWater( true );

			// Underwater stamina is consumed as oxygen
			fStamina -= fRunStamina * 2.0f;
		}

	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Movement Update
	{
		switch ( iMotionType ) {
			case LIVE_ENTITY::Motion::Walk:		{ this->PhysicUpdate_Walk();	break; }
			case LIVE_ENTITY::Motion::Fly:		{ this->PhysicUpdate_Fly();		break; }
			case LIVE_ENTITY::Motion::Swim:		{ this->PhysicUpdate_Swim( bIsEntityInWater, bIsCameraUnderWater, bIsCameraReallyUnderWater );	break; }
			case LIVE_ENTITY::Motion::P1ToP2:	{ this->PhysicUpdate_P1ToP2();	break; }
		}
	}

	// Update flashlight position and rotation
	pFlashLight->Update();

	////////////////////////////////////////////////////////////////////////////////////////
	// Update all States
	{
		this->SetState( LIVE_ENTITY::Actions::Crouched, States.bIsCrouched	);

		this->SetState( LIVE_ENTITY::Actions::Moving,	States.bIsMoving	);
		this->SetState( LIVE_ENTITY::Actions::Walking,	States.bIsWalking	);
		this->SetState( LIVE_ENTITY::Actions::Running,	States.bIsRunning	);

		this->SetState( LIVE_ENTITY::Actions::Jumping,	States.bIsJumping	);
		this->SetState( LIVE_ENTITY::Actions::Hanging,	States.bIsHanging	);
		this->SetState( LIVE_ENTITY::Actions::Falling,	States.bIsFalling	);
	}

//	OnFrame(); // draw usefull infos

}

// Public
// Update movements
void cActor::OnFrame( void ) {

	Engine()->DrawInfo( "IsIdle %s",	IsIdle() ? "true" : "false" );
	Engine()->DrawInfo( "IsMoving %s",	IsMoving() ? "true" : "false" );
	Engine()->DrawInfo( "IsLeaning %s",	IsLeaning() ? "true" : "false" );
	Engine()->DrawInfo( "IsWalking %s",	IsWalking() ? "true" : "false" );
	Engine()->DrawInfo( "IsRunning %s",	IsRunning() ? "true" : "false" );
	Engine()->DrawInfo( "IsJumping %s",	IsJumping() ? "true" : "false" );

	Engine()->DrawInfo( "IsHanging %s",	IsHanging() ? "true" : "false" );
	Engine()->DrawInfo( "IsFalling %s",	IsFalling() ? "true" : "false" );
	Engine()->DrawInfo( "IsCrouched %s",IsCrouched() ? "true" : "false" );
	
}
