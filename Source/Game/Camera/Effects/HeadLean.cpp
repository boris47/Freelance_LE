

#include "HeadLean.h"

#include "Game\Camera\CamManager.h"
#include "Game\Level\World.h"


// Constructor
cHeadLean::cHeadLean( cSection *CameraSection ) {

	fCFG_Lean_Max	= CameraSection->GetFloat( "HeadLean_Max_Deegree",	15.0 );

}


// Public
void cHeadLean::Update( cEntity *CameraParent ) {

	// Skip if not active
	if ( !bActive ) return;

	char iLeanDir = Engine()->InputManager()->LeanLeft() - Engine()->InputManager()->LeanRight();
	float fLeanTarget = ( iLeanDir != 0 ) ? iLeanDir * ( fCFG_Lean_Max * fAmplitudeMul ) : ZERO_FLOAT;

	// Get parent and set the lean state
	cLiveEntity *pLiveEntity = CameraParent->GetAsLiveEntity();
	pLiveEntity->SetState( LIVE_ENTITY::Actions::Leaning, ( iLeanDir != 0 ) ? true : false );

	vRotation.z = Leadwerks::Math::Curve( fLeanTarget, vRotation.z, 12 );

	vPosition = Leadwerks::Transform::Vector( -vRotation.z / 60, 0, 0, CamManager()->GetCamera(), NULL );

}


// Public
void cHeadLean::Reset( bool bInstant ) {

	if ( bInstant ) {
		vPosition.x = vPosition.z = vRotation.z = ZERO_FLOAT;
	} else {
		// Adjust position after leaning ( interrupt leaning )
		if ( vPosition.Length() != ZERO_FLOAT ) vPosition = Vec3Curve( ZERO_FLOAT, vPosition, 20 );

		// Adjust rotation after leaning ( interrupt leaning )
		if ( vRotation.z != ZERO_FLOAT ) vRotation.z = Leadwerks::Math::Curve( ZERO_FLOAT, vRotation.z, 10 );
	}

}
