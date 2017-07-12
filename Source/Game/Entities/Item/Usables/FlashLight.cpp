
#include "FlashLight.h"
#include "Game\Level\Level.h"

cAudioSrc *pSound = NULL;

// Constructor
cFlashLight::cFlashLight() {

	cSection *pSection = LTXReader.GetSection( "Flashlight" );
	if ( !pSection ) {
		cMsgCRT( "cFlashLight::cFlashLight:Cannot find Flashlight section" );
	}
	
	vRelPos = pSection->GetVec3( "Position" );
	pSound = Engine()->AudioDevice()->FindSource( GetMultiString1( pSection, "Sound", 1 ) );
	if ( pSound )
		pSound->SetVolume( GetMultiFloat1( pSection, "Sound", 2 ) );

	Leadwerks::SpotLight *pLight = Leadwerks::SpotLight::Create();
	pLight->SetRange(	GetMultiFloat2( pSection, "Range", 1, 2 ) );
	pLight->SetConeAngles(	GetMultiFloat2( pSection, "ConeAngles", 1, 2 ) );
	pLight->SetShadowMode(	Leadwerks::Light::Static + Leadwerks::Light::Dynamic );
	pLight->Show(); // precache light
	pLight->Hide();
	pEntity = pLight;
}


cFlashLight::~cFlashLight() {
	SAFE_DELETE( pEntity );
}


// Public
void cFlashLight::Update() {
	
	if ( !pParent ) return;

	// If pressed command relative to "Item1"
	if ( Engine()->InputManager()->Item1() ) {
		if ( pEntity && IsEnabled() ) {
			IsActive() ? Deactivate() : Activate();
			Engine()->AudioDevice()->Play2D( pSound );
		}
	}

	float fVolumetricEffect = Level()->WeatherManager()->GetFogEffect()->GetFactor();
	dynamic_cast<Leadwerks::SpotLight *>( pEntity )->SetVolumetricStrength( fVolumetricEffect );

	// Update Flashlight rotation
	Leadwerks::Vec3 vParentRotation		= pParent->GetRotation();

	Leadwerks::Vec3 vOldFlashlightDir	= pEntity->GetRotation();
	vDirection = Vec3Curve( vParentRotation + vRelDir, vOldFlashlightDir, 3 );
	pEntity->SetRotation( vDirection );

	// Update Flashlight position
	Leadwerks::Vec3 vParentPosition		= pParent->GetPosition();
	Leadwerks::Vec3 vRelPosition		= Leadwerks::Transform::Vector( vRelPos, pParent, NULL );
	vPosition = vParentPosition + vRelPosition;
	pEntity->SetPosition( vPosition );

}
