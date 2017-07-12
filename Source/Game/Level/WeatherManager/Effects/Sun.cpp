
#include "Engine.h"

#include "Sun.h"
#include "Game\Level\World.h"
#include "Game\Camera\CamManager.h"
#include "..\WeatherManager.h"

// Spotlight range, bad idea changing this
#define SPOTLIGHT_MAXRANGE 50.0f
// value subtracted to sun elevation for start end volumetric effect
#define SPOTLIGTH_HEIGTH_CORRECTION 0.15f
// base value of volume effect
#define SPOTLIGHT_VOLUME_BASE 0.7f

cSunEffect::cSunEffect() {

	// This is used for volumetric effect
	pLight = Leadwerks::SpotLight::Create();

	// Intensity is set to zero to prevent shadow drawing
	// i'll use specular color to set real light color
	pLight->SetIntensity( 0.0 );

//	pLight->SetViewRange( Leadwerks::Light::MediumViewRange );
	pLight->SetViewRange( Leadwerks::Light::MaxViewRange );
	pLight->SetConeAngles( 8.0, 9.0 );
	pLight->SetShadowMode( Leadwerks::Light::Static + Leadwerks::Light::Dynamic );

	bIsOK = true;
	
}

cSunEffect::~cSunEffect() {

	SAFE_RELEASE( pShader );
	SAFE_RELEASE( pMoonSprite );

}

bool cSunEffect::Initialize() {

	bIsInitialized = true;
	return true;

}

void cSunEffect::Update( Leadwerks::Vec3 vSunRotation ) {

	// true if sun is visible
	bool bSunInSky = fGradientRadius > ZERO_FLOAT;

	// sun intesity [ 0.0 .. 1.0 ] based on sun elevation
	float fSunBright = Leadwerks::Math::Max( -vSunRotation.y - SPOTLIGTH_HEIGTH_CORRECTION, 0.0 );

	Leadwerks::Camera *pCamera = CamManager()->GetCamera();

	// SpotLight update
	{
		////////////////////////////////////////////////////////////////////////////////////////
		// Range is base on the sun rise and sun set making rays longer than top sun elevation ammount
		// During sunset and sunrise range is doble than when the sun is at top elevation
		float fRange = SPOTLIGHT_MAXRANGE /*+ 2.0*/ + ( SPOTLIGHT_MAXRANGE * ( 1.0 - fGradientRadius ) );
		pLight->SetRange( 0.02, fRange );

		////////////////////////////////////////////////////////////////////////////////////////
		// Volumetric strength is based on sun elevation [ 0.0 .. 1.0 ]
		pLight->SetVolumetricStrength( SPOTLIGHT_VOLUME_BASE * ( bSunInSky ? fSunBright : 0.0f ) );

		////////////////////////////////////////////////////////////////////////////////////////
		// Light color is a mix of a base color ( a reddish tone ) faded thanks to sun elevation
		// vSunColor + reddish tone on sunset and surise, clear color on top elevation
		Leadwerks::Vec3 vVolColor = vSunColor + ( Leadwerks::Vec3 (2.55, 1.41, 0.76 ) * ( 1.0 - fSunBright ) );
		pLight->SetColor( Leadwerks::Vec4( vVolColor, float( iQuality ) ), Leadwerks::COLOR_SPECULAR );

		////////////////////////////////////////////////////////////////////////////////////////
		// Ligth position is relative to sun elevation
		// During sunrise and sunset this light is at double distance than with sun at top elevation
		Leadwerks::Vec3 vLightPosition = pCamera->GetPosition( true ) - ( vSunRotation * ( fRange * 0.9 ) );
		pLight->SetPosition( vLightPosition );
		pLight->AlignToVector( vSunRotation );
	}

	// sun shader update
	{
		Leadwerks::DirectionalLight *pSun = Level()->WeatherManager()->GetSun();
		Leadwerks::Buffer *pBuffer = Leadwerks::Buffer::GetCurrent();
		Leadwerks::Vec3 vSunPosition = pCamera->GetPosition( true ) - ( vSunRotation * SPOTLIGHT_MAXRANGE );
		Leadwerks::Vec3 vSunScreenPos = pCamera->Project( vSunPosition );
		Leadwerks::Vec3 vSunRotation = Leadwerks::Transform::Vector( 0, 0, 1, pSun, pCamera );

		vSunScreenPos.x /= pBuffer->GetWidth();
		vSunScreenPos.y /= pBuffer->GetHeight();

		if ( pShader ) {
			pShader->SetInt	 ( "iSunQuality",	iQuality		);
			pShader->SetFloat( "fSunBright",	fSunBright		);
			pShader->SetVec3 ( "vSunScreenPos", vSunScreenPos	);
			pShader->SetVec3 ( "vSunColor",		vSunColor		);
			pShader->SetVec3 ( "vSunRotation",	vSunRotation	);

			pShader->SetFloat( "fGlowSize",		fGradientRadius );
		}
	}
}