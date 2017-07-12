
#include "HeadShake.h"

#include "Game\Camera\CamManager.h"

/*
	To DO
	
*/

#define THETA_X_UPDATE				  0.08f
#define THETA_Y_UPDATE				  0.03f

#define SPEED_MAX					 20.00f
#define SPEED_MIN					  1.00f

#define AMPLITUDE_MAX				 30.00f
#define AMPLITUDE_DEC				  2.00f

void cHeadShake::Shake( float fAmplitudeMul, float fSpeedMul, long iDuration ) {

	fAmplitude *= fAmplitudeMul;
	fXSpeed *= fSpeedMul;
	fYSpeed *= fSpeedMul;

	this->iDuration = iDuration;
	iTime = Leadwerks::Time::GetCurrent() + iDuration;
}


void cHeadShake::Update( void ) {

	if ( !bActive ) return;

	fXTheta += THETA_X_UPDATE * fXSpeed;
	fYTheta += THETA_Y_UPDATE * fYSpeed;

}


void cHeadShake::OnFrame( void ) {
	/*
		aggiornato il valore fi fAmplitude la condizione è subito valuda pochi passaggi oltre
		questo rimodifica i valore subito dopo e così l' ampiezza arriva subito a zero
	*/
//	 when reach max amplitude
	if ( Similar( abs( vTargetDir.x ), fAmplitude ) ) {
		float fSpeed = Leadwerks::Math::Random( SPEED_MIN, SPEED_MAX );
		fXSpeed = fSpeed;
		fYSpeed = fSpeed * Leadwerks::Math::Random( 0.03 );
		fAmplitude -= AMPLITUDE_DEC;
	}


}


void cHeadShake::Reset( bool bInstant ) {
	Disable();
	if ( bInstant ) {
		vDirection.x = vDirection.y = vDirection.z = ZERO_FLOAT;
	} else {
		if ( vDirection.Length() != ZERO_FLOAT ) vDirection = Vec3Curve( ZERO_FLOAT, vDirection, 10 );
	}
}
