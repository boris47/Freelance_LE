

#include "HeadBob.h"

#include "Game\Camera\CamManager.h"
#include "Game\Level\World.h"


// Constructor
cHeadBob::cHeadBob( cSection *CameraSection ) {

	fCFG_Amplitude	= CameraSection->GetFloat( "HeadBob_Amplitude",		0.5 );
	fCFG_Speed		= CameraSection->GetFloat( "HeadBob_Speed",			4.5 );

	fCFG_Step_Value	= CameraSection->GetFloat( "HeadBob_Step",			0.9 );

	fTheta_Update	= CameraSection->GetFloat( "HeadBob_Theta_Update",	1.2 );

	fCFG_Step_Value = Leadwerks::Math::Clamp( fCFG_Step_Value, 0.0f, 1.0f );

	fStepValue = fCFG_Step_Value;
}


// Public
// Apply head bobbing effect
void cHeadBob::Update( cEntity *CameraParent ) {

	// Skip if not active
	if ( !bActive ) return;

	// Get parent info
	cLiveEntity *pLiveEntity	= CameraParent->GetAsLiveEntity();
	bool bCrouched				= pLiveEntity->IsCrouched();
	bool bRunning				= pLiveEntity->IsRunning();
	float fStamina				= pLiveEntity->GetStamina();

	// Camera zoom state
	bool bZoomed = CamManager()->GetZoomEffect()->IsZoomed();

	// Modifiers
	float fSpeed = fCFG_Speed * fSpeedMul;
	fSpeed *= ( ( bRunning )	?1.7:1.0 );
//	fSpeed *= ( ( bCrouched )	?0.5:1.0 );
//	fSpeed *= ( ( bZoomed )		?0.5:1.0 );

	float fAmplitude = fCFG_Amplitude * fAmplitudeMul;
//	fAmplitude *= ( ( bRunning )	? 2.0:1.0 );
	fAmplitude *= ( ( bCrouched )	? 0.5 : 1.0 );
//	fAmplitude *= ( ( bZoomed )		? 0.8:1.0 );
	fAmplitude *= ( 3.f - fStamina * 2 );

	// Calculate speed
	fThetaX += fTheta_Update * fSpeed;
	fThetaY += ( fTheta_Update / 2 ) * fSpeed;
	if ( fThetaX > 360 ) fThetaX -= 360;
	if ( fThetaY > 360 ) fThetaY -= 360;

	float fCosThetaX = Leadwerks::Math::Cos( fThetaX );
	float fCosThetaY = Leadwerks::Math::Cos( fThetaY );

	vTargetDir.x = fCosThetaX * fAmplitude;
	vTargetDir.y = fCosThetaY * fAmplitude;

	vTargetPos = Leadwerks::Transform::Vector( -vTargetDir.x / 50, 0, 0, CamManager()->GetCamera(), NULL );

	// Set relative position and direction
	vPosition = Vec3Curve( vTargetPos, vPosition, 5 );
	vRotation = Vec3Curve( vTargetDir, vRotation, 2 );

	// Make steps
//	fStepValue = Leadwerks::Math::Max( fStepValue, fCosThetaX );
	if ( abs( fCosThetaY ) > ( fStepValue ) ) {
		if ( !bStepDone ) {
			{
				// try to retrieve info from ground type and use them for footsteps sound
				Leadwerks::PickInfo Info;
				Leadwerks::Vec3 vParentPos = CameraParent->GetEntity()->GetPosition();
				Leadwerks::Vec3 vDestPos = vParentPos - Leadwerks::Vec3( 0.0, 2.0, 0.0 );
				bool bPick = World()->GetWorld()->Pick( vParentPos, vDestPos, Info/*, 0.0, false, Leadwerks::Collision::LineOfSight*/ );
				if ( bPick ) pLiveEntity->GetFootsetpManager()->Step( Info, bCrouched, bRunning );
				bStepDone = true;
			}
		}
	} else bStepDone = false;

}


// Public
void cHeadBob::Reset( bool bInstant ) {

	if ( bInstant ) {
		vPosition.x = vPosition.y = vPosition.z = ZERO_FLOAT;
	} else {
		// Adjust position after headbobbing ( interrupt headbobbing )
//		if ( vPosition.Length() != ZERO_FLOAT ) vPosition = Vec3Curve( ZERO_FLOAT, vPosition, 20 );
	
		// Adjust rotation after headbobbing ( interrupt headbobbing )
//		if ( vDirection.x != ZERO_FLOAT ) vDirection.x = Leadwerks::Math::Curve( ZERO_FLOAT, vDirection.x, 10 );

//		if ( fTheta != ZERO_FLOAT ) fTheta = L_CURVE( ZERO_FLOAT, fTheta, 10 );
	}

}