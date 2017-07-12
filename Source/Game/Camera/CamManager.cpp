

#include "CamManager.h"
#include "Game\\Level\\Level.h"
#include "Audio\Audio.h"	// LeadwerksV3TocAudioVec( Leadwerks::Vec3 v )

#define MAX_POINTED_DISTANCE	127.00f


cCamManager *pCamManager = NULL;

inline void CameraUpdateHook(  ) { CamManager()->Update(); }

// Constructor
cCamManager::cCamManager() {

	cMsgDBG( "cCamManager::Constructor" );
	////////////////////////////////////////////////////////////////////////////////////////
	// Leadwerks Camera Creation
	{	    
		pCamera = Leadwerks::Camera::Create();
		if ( !pCamera ) {
			cMsgCRT( "cCamManager::Constructor:Cannot create Leadwerks camera" );
			return; // leave bIsOK false
		}
		pCamera->SetCollisionType( Leadwerks::Collision::None );
		pCamera->SetPickMode( 0 );
		pCamera->SetShadowMode( 0 );
		pCamera->SetKeyValue( "name", "Camera" );
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Setup of camera
	{
		cSection *pCamSection = Settings.GetSection( "Camera" );
		pCamera->SetFOV					( L_CLAMP( pCamSection->GetFloat( "FOV", 70.0 ), 60.f, 100.f ) );
		fMouseSensitivity				= L_CLAMP( pCamSection->GetFloat( "MouseSensitivity", 1.5 ), 0.5, 5.0 );
		pCamera->SetMultisampleMode		( L_CLAMP( pCamSection->GetInt( "MultiSample", 1 ), 1, 16 ) );
	}
	{
		cSection *pSection = LTXReader.GetSection( "Camera" ); // Inside Actor.ltx
		fCamHeigth						= pSection->GetFloat( "CamHeigth",				1.9 );
		fCamInterpolation				= pSection->GetFloat( "CamInterpolation",		0.0 );
		fFlySpeed						= pSection->GetFloat( "FlySpeed",				3.0 );
		fFlyBoost						= pSection->GetFloat( "FlyBoost",				2.0 );

		float fViewDistance				= pSection->GetFloat( "ViewDistance",			4096.f );
		pCamera->SetRange( 0.05,		L_CLAMP( fViewDistance, 50.f, 4096.f ) );

		// Store internal reference of Camera section
		this->SetSection( pSection );
	}

	cMsgDBG( "cCamManager::Constructor OK" );
	bIsOK = true;
}


// Public
cCamManager::~cCamManager() {
	
	
	SAFE_DELETE( pPointer );
	
	// Destroy Camera Effects
	{
		SAFE_DELETE( pHeadBob );
		SAFE_DELETE( pHeadMove );
		SAFE_DELETE( pHeadLean );
	}

	SAFE_DELETE( pZoom );
	SAFE_RELEASE( pCamera );

}

/*
	TO DO: ISOLATE CAMERA FROM PARENT
*/

// Public
bool cCamManager::Initialize( cEntity *Parent ) {

	cMsgDBG( "cCamManager::Initialize" );

	if ( bIsInitialized ) return true; // Just already initialized

	if ( !bIsOK ) {
		cMsgCRT( "cCamManager::Initialize:Camera bad creation" );
		return false; // leave bIsInitialized = false
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Parenting
	{
		if ( !pParent && !Parent ) {
			cMsgCRT( "cCamManager::Initialize:Camera must have a parent at or before initialization" );
			return false; // leave bIsInitialized = false
		}

		if ( ( pParent && !pParent->GetEntity() ) || ( Parent && !Parent->GetEntity() ) ) {
			cMsgCRT( "cCamManager::Initialize:Parent of camera has no leadwerks entity" );
			return false; // leave bIsInitialized = false
		}

		// Now that Parent is a valid entity..
		if ( !pParent ) pParent = Parent;

		pCamera->SetRotation( Parent->GetEntity()->GetRotation() );
		vCamRotation = Parent->GetEntity()->GetRotation();

		Level()->GetActor()->GetFlashLight()->SetParent( pCamera );
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Pointer
	{
		pPointer = new cPointer( pCamera );
		pPointer->SetMaxDistance( MAX_POINTED_DISTANCE );
		pPointer->SetDirection( { 0, 0, 1 } );
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Camera Effects
	{
		cSection *pEffSection = Settings.GetSection( "Effects" );
		{
			pHeadBob = new cHeadBob( this->GetSection() );
			if ( !pEffSection->GetBool( "HeadBob" ) ) pHeadBob->Disable();	// Enabled by default
		}
		{
			pHeadMove = new cHeadMove( this->GetSection() );
			if ( !pEffSection->GetBool( "HeadMove" ) ) pHeadMove->Disable();// Enabled by default
		}
		{
			pHeadLean = new cHeadLean( this->GetSection() );
		}
		{
///			pHeadShake = new cHeadShake( this->GetSection() );
///			if ( !pEffSection->GetBool( "HeadShake" ) ) pHeadShake->Disable();// Enabled by default
		}
		{
			pZoom = new cZoom( pCamera );
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Post-Processing
	{
		cSection *pCamSection = Settings.GetSection( "Camera" );
//		if ( !LoadPostEffect( "Shaders\\PostEffects\\00_Underwater_Caustics.lua"	) ) return false;
		if ( !LoadPostEffect( "Shaders\\PostEffects\\02_pp_fog_by_klepto.lua"		) ) return false;
/*		if( pCamSection->GetBool( "DOF" ) )
			if ( !LoadPostEffect( "Shaders\\PostEffects\\04_pp_dof.lua"				) ) return false;
		if ( pCamSection->GetBool( "HDR" ) )
			if ( !LoadPostEffect( "Shaders\\PostEffects\\06_pp_hdr.lua"				) ) return false;
		if ( pCamSection->GetBool( "SunEffects" ) )*/
			if ( !LoadPostEffect( "Shaders\\PostEffects\\09_pp_suneffects.lua"		) ) return false;
		if ( pCamSection->GetBool( "Bloom" ) )
			if ( !LoadPostEffect( "Shaders\\PostEffects\\08_pp_bloom.lua"			) ) return false;
//		if ( pCamSection->GetBool( "SSDO" ) )
//			if ( !LoadPostEffect( "Shaders\\PostEffects\\10_pp_ssdo.lua"			) ) return false;
		if ( pCamSection->GetBool( "FXAA" ) )
			if ( !LoadPostEffect( "Shaders\\PostEffects\\99_pp_fxaa.shader"			) ) return false;
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// By default already set, just for clarity
	SetCameraFirstPerson();
	////////////////////////////////////////////////////////////////////////////////////////
	// Register Update Hook
//	pCamera->AddHook( Leadwerks::Entity::UpdatePhysicsHook, CameraUpdateHook );
	Engine()->RegisterPeriodicHook( 1000 / 60, CameraUpdateHook ); // 60 times per socond, every 16.66666ms

	cMsgDBG( "cCamManager::Initialize OK" );
	bIsInitialized = true;
	return true;
}

// Private
bool cCamManager::LoadPostEffect( std::string FilePath ) {

	if ( !bIsOK ) return false;

	if ( !FileExists( FilePath ) ) {
		cMsgCRT( "cCamManager::LoadPostEffect:Cannot find file " + FilePath );
		return false;
	}

	if ( GetExtFromFilePath( FilePath ) == ".lua" ) {
		if ( pCamera->AddPostEffect( FilePath ) == -1 ) {
			cMsgCRT( "cCamManager::LoadPostEffect::Cannot add post porcess " + FilePath );
			return false;
		}
	} else {
		if ( !Leadwerks::Shader::Load( FilePath ) ) {
			cMsgCRT( "cCamManager::LoadPostEffect::Cannot load shader " + FilePath );
			return false;
		}
	}

	return true;
}


// Public
bool cCamManager::SetParent( cEntity *Parent ) {

	if ( !Parent ) {
		cMsgCRT( "cCamManager::SetParent:Parent == NULL" );
		return false;
	}

	if ( !Parent->GetEntity() ) {
		cMsgCRT( "cCamManager::Initialize:Parent has no entity inialized" );
		return false;
	}

	pParent = Parent;
	pCamera->SetRotation( pParent->GetEntity()->GetRotation() );

	return true;
}

// Public
bool cCamManager::Detach( void ) {

	if ( pParent ) {
		pOldParent = pParent;
		pParent = NULL;
		return true;
	}
	return false;

}

// Public
bool cCamManager::Attach( void ) {

	if ( pOldParent ) {
		pParent = pOldParent;
		return true;
	}
	return false;

}

// Private
void cCamManager::UpdateFreeMode( void ) {

	cInputManager *pIM			= Engine()->InputManager();
	bool bRunButtonHolden		= pIM->Run();
	float fMove					= pIM->Forward() - pIM->Backward();
	float fStrafe				= pIM->StrafeRight() - pIM->StrafeLeft();
	float fVertical				= pIM->Jump() - pIM->Crouch();

	// Modifier
	float fSpeed = fFlySpeed * ( bRunButtonHolden ? fFlyBoost : 1.0 );

	fMove *= fSpeed;
	fStrafe *= fSpeed;
	fVertical *= fSpeed * 0.2;

	pCamera->Move( fStrafe, fVertical, fMove );

}


// Private
void cCamManager::UpdateRotation( void ) {

	Leadwerks::Window *pWindow = Leadwerks::Window::GetCurrent();
	Leadwerks::Context *pContext = Leadwerks::Context::GetCurrent();
	float fMiddleWidth = pContext->GetWidth() / 2, fMiddleHeight = pContext->GetHeight() / 2;

	// CAPTURE MOUSE MOTION
	bRotating = false;
	////////////////////////////////////////////////////////////////////////////////////////
	// Mouse motion
	{
		// Vars that will contains mouse motion quanitiies
		float fMoveX = 0.0f, fMoveY = 0.0f;

		// If mouse is moved calculate motion quantity
		Leadwerks::Vec3 vMousePosition = pWindow->GetMousePosition();
		if ( ( abs( vMousePosition.x - fMiddleWidth  ) > 1.0f ) || ( abs( vMousePosition.y - fMiddleHeight ) > 1.0f ) ) {
			fMoveX = ( vMousePosition.x - fMiddleWidth  ) * ( fMouseSensitivity / 10 ) / pZoom->GetCamZoom();
			fMoveY = ( vMousePosition.y - fMiddleHeight ) * ( fMouseSensitivity / 10 ) / pZoom->GetCamZoom();
			// Mouse motion flag
			bRotating = true;
		}

		// Set mouse position at the center of screen
		pWindow->SetMousePosition( fMiddleWidth, fMiddleHeight );

		// If interpolation is > 0, create soft mouse movement interpolation with previous motions
		if ( fCamInterpolation >= 1.0f ) {
			fCamDifferenceX = L_CURVE( fMoveX, fCamDifferenceX, fCamInterpolation );
			fCamDifferenceY = L_CURVE( fMoveY, fCamDifferenceY, fCamInterpolation );
		} else {
			fCamDifferenceX = fMoveX;
			fCamDifferenceY = fMoveY;
		}
	}

	// If mouse has been moved
	if ( bRotating ) {

		// Clamp vertical movement in range [ -80, 80 ] degree
		float fNewCamRotation_X = L_CLAMP( vCamRotation.x + fCamDifferenceY, -80.0f, 80.0f );
		float fNewCamRotation_Y = vCamRotation.y + fCamDifferenceX;
		float fNewCamRotation_Z = vCamRotation.z;

		// Assign internal camera rotation
		vCamRotation = { fNewCamRotation_X, fNewCamRotation_Y, fNewCamRotation_Z };

	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Get effects modifiers
	{
		Leadwerks::Vec3 vHeadBobDir		= pHeadBob->GetRotation();
		Leadwerks::Vec3 vHeadMoveDir	= pHeadMove->GetRotation();
		Leadwerks::Vec3 vLeanDir		= pHeadLean->GetRotation();
		vCamRelRotation = vHeadBobDir  +  vHeadMoveDir  +  vLeanDir;
	}

	// Update camera rotation
	pCamera->SetRotation( vCamRotation + vCamRelRotation );

//	// update parent rotation
	if ( pParent && !pParent->IsAirborne() ) {
		pParent->GetEntity()->SetRotation( pCamera->GetRotation() );
	}


}

// Private
void cCamManager::UpdatePosition( void ) {

	// Base cam position
	vCamPosition = pParent->GetEntity()->GetPosition();

	cLiveEntity * pLiveEntity = pParent->GetAsLiveEntity();
	bool bIsCrouched	= pLiveEntity->IsCrouched();
	bool bIsUnderWater	= pLiveEntity->IsUnderWater();
	fCamShift = L_CURVE( ( ( bIsCrouched ) ? 0.5:1.0 ), fCamShift, 5.0f );

	// smooth stand-up->cruoch and viceversa transition
	vCamPosition.y += ( fCamHeigth * fCamShift );

	// Get effects modifiers
	Leadwerks::Vec3 vHeadBobPos = pHeadBob->GetPosition();
	Leadwerks::Vec3 vLeanPos	= pHeadLean->GetPosition();
	vCamRelPosition = vLeanPos + vHeadBobPos;
	
	// Update camera position
	pCamera->SetPosition( vCamPosition + vCamRelPosition );

}

// Public
void cCamManager::Update( void ) {

	// Update all camera effects
	bool bZoomed		= pZoom->IsZoomed();

	////////////////////////////////////////////////////////////////////////////////////////
	// If camera is attached to a parent, update data
	if ( pParent ) {

		////////////////////////////////////////////////////////////////////////////////////////
		// Retrieve parent states
		cLiveEntity *	  pLiveEntity = pParent->GetAsLiveEntity();
		bool bMoving	= pLiveEntity->IsMoving();
		bool bIsHanging = pLiveEntity->IsHanging();
		bool bIsFalling = pLiveEntity->IsFalling();

		////////////////////////////////////////////////////////////////////////////////////////
		// Update Effects
		{
			bool bIsPlainWalking	= pLiveEntity->IsPlainWalking();
			bool bIsSwimming		= pLiveEntity->IsSwimming();
			bool bIsFlying			= pLiveEntity->IsFlying();
			
			if ( bMoving ) {
				if ( !bIsHanging && !bIsFalling && bIsPlainWalking )
					pHeadBob->Update( pParent );
				else
					pHeadBob->Reset();
				pHeadMove->Reset();
				pHeadLean->Reset();

			} else {
				pHeadBob->Reset();
				pHeadMove->Update( pParent );

				// Lean allowed only if on plain walk motion
				if ( !bIsHanging && !bIsFalling && !bIsSwimming && !bIsFlying )
					pHeadLean->Update( pParent );
			}

			pZoom->Update();
		}
		// Update position by parent
		this->UpdatePosition();

	}

	////////////////////////////////////////////////////////////////////////////////////////
	// If free mode, update camera position
	{
		if ( HasState( CAMERA_STATES::Free ) )
			UpdateFreeMode();
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Update position by parent
	this->UpdateRotation();

	////////////////////////////////////////////////////////////////////////////////////////
	// Update 3D audio listener position
	{
		Leadwerks::Listener *pListener = Engine()->AudioDevice()->GetListener();
		if ( pListener ) {
			pListener->Move( pCamera->GetPosition() );
			pListener->SetRotation( pCamera->GetRotation() );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Pointer
	// Calulate Distance from pointed position in world
	{
		pPointer->SetPosition( vCamPosition );
		pPointer->Update();
		fPointedDistance = pPointer->GetDistance();
	}

}