
#include "Engine.h"

#include "Fog.h"
#include "Game\Level\World.h"
#include "Game\Camera\CamManager.h"

#define BASE_FOG_COLOR Leadwerks::Vec3( 0.17f, 0.13f, 0.15f )


cFogEffect::cFogEffect() {

	bIsOK = true;

}

cFogEffect::~cFogEffect() {

	SAFE_RELEASE( pShader );

}

bool cFogEffect::Initialize() {

	bIsInitialized = true;
	return true;

}

void cFogEffect::Update( void ) {
	
	if ( !pShader ) return;

	float fFogRange = CamManager()->GetCamera()->range.y / 3.0;
	Leadwerks::Vec3 vFogColor = BASE_FOG_COLOR * vColorMult;
	float fFogDensity = 0.8 * fFactor; // alpha of fog color

	pShader->SetInt		(	"IsActive",		(fFactor > 0.0f)?1:0);
	pShader->SetFloat	(	"fFogRange",	fFogRange			);
	pShader->SetVec3	(	"vFogColor",	vFogColor			);
	pShader->SetFloat	(	"fFogDensity",	fFogDensity			);


}