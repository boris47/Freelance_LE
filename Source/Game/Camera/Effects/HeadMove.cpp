

#include "HeadMove.h"

#include "Game\Camera\CamManager.h"
#include "Game\Level\World.h"


// Constructor
cHeadMove::cHeadMove( cSection *CameraSection ) {

	fCFG_Amplitude		= CameraSection->GetFloat( "HeadMove_Amplitude",		0.5 );
	fCFG_Speed			= CameraSection->GetFloat( "HeadMove_Speed",			4.5 );

	fTheta_Update_Vert	= CameraSection->GetFloat( "HeadMove_Theta_Upd_Vert",	0.9 );
	fTheta_Update_Oriz	= CameraSection->GetFloat( "HeadMove_Theta_Upd_Oriz",	0.2 );

}


// Public
// Apply head move effect
void cHeadMove::Update( cEntity *CameraParent ) {
	
	// Skip if not active
	if ( !bActive ) return;

	// Get parent info
	cLiveEntity *pLiveEntity	= CameraParent->GetAsLiveEntity();
	bool bCrouched				= pLiveEntity->IsCrouched();
	bool bIsUnderwater			= pLiveEntity->IsUnderWater();
	bool bIsMoving				= pLiveEntity->IsMoving();
	float fStamina				= pLiveEntity->GetStamina();

	// Camera zoom state
	bool bZoomed = CamManager()->GetZoomEffect()->IsZoomed();

	// Modifiers
	float fSpeed = fCFG_Speed * fSpeedMul;
	fSpeed *= ( bCrouched )		? 0.8f  : 1.0f;
	fSpeed *= ( bIsUnderwater )	? 0.5f  : 1.0f;
	fSpeed *= ( bZoomed		  ) ? 0.85f : 1.0f;
	fSpeed *= ( 4.f - fStamina*2.f );

	float fAmplitude = fCFG_Amplitude * fAmplitudeMul;
	fAmplitude *= ( ( bCrouched )	? 0.8f  : 1.f );
	fAmplitude *= ( ( bZoomed )		? 0.85f : 1.f );

	// only adjust amblitude if not underwater
	if ( !bIsUnderwater )
		fAmplitude *= ( 5.f - fStamina*4.f );
	else
		// else underwater, only while moving, increase amplitude
		if ( bIsMoving )
			fAmplitude *= 5.f;

	// Calculate speed
	fThetaX += fTheta_Update_Vert * fSpeed;
	fThetaY += ( fTheta_Update_Oriz + Leadwerks::Math::Random( 0.03 ) ) * fSpeed;
	if ( fThetaX > 360 ) fThetaX -= 360;
	if ( fThetaY > 360 ) fThetaY -= 360;

	// Set relative direction
	vTargetRot.x = -Leadwerks::Math::Cos( fThetaX ) * fAmplitude;
	vTargetRot.y =  Leadwerks::Math::Cos( fThetaY ) * fAmplitude * 0.2;

	vRotation = Vec3Curve( vTargetRot, vRotation, 10 );

}

// Public
void cHeadMove::Reset( bool bInstant ) {

	if ( bInstant ) {
		vRotation.x = vRotation.y = vRotation.z = ZERO_FLOAT;
	} else {
		if ( vRotation.Length() != ZERO_FLOAT ) vRotation = Vec3Curve( ZERO_FLOAT, vRotation, 10 );
	}

}