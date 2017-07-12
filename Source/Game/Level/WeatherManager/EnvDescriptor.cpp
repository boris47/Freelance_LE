
#include "EnvDescriptor.h"


cEnvDescriptor::cEnvDescriptor( std::string Identifier, float Time ) {
	 fExecTime = Time;
	 sIdentifier = Identifier;
}


cEnvDescriptor::~cEnvDescriptor() {

	SAFE_RELEASE( pSunMaterial );
	SAFE_RELEASE( pGradientMaterial );
	SAFE_RELEASE( pSkyTexture );

}


bool cEnvDescriptor::Load( cSection *Section ) {
	const char *DescriptorName = sIdentifier.c_str();
//	cMsgDBG( "LOADING DESCRIPTOR %s with  fExecTime %f", DescriptorName, fExecTime );

	// Ambient Color
	if ( !Section->bGetVec3( "ambient_color", vAmbientColor, { 1.0f, 1.0f, 1.0f } ) ) 
		cMsgCRT( "cEnvDescriptor::Load:Cannot retrieve \"ambient_color\" for descriptor %s, setting default", DescriptorName );

	// Fog
	if ( !Section->bGetFloat( "fog_factor", fFogFactor, 0.0f ) )
		cMsgCRT( "cEnvDescriptor::Load:Cannot retrieve \"fog_factor\" for descriptor %s, setting default", DescriptorName );

	// Sky Texture
	if ( !Section->bGetString( "sky_texture", sSkyTexture ) || ( !sSkyTexture.length() ) ) {
		cMsgCRT( "cEnvDescriptor::Load:Invalid \"sky_texture\" in descriptor %s", DescriptorName );
		return false;
	}
	//	vSkyColor;
	if ( !Section->bGetVec3( "sky_color", vSkyColor, { 1.0, 1.0, 1.0 } ) )
		cMsgCRT( "cEnvDescriptor::Load:Cannot retrieve \"sky_color\" for descriptor %s, setting default", DescriptorName );
	


	// Gradient Radius
	if ( !Section->bGetFloat( "gradient_radius", fGradientRadius, 0.0f ) )
		cMsgCRT( "cEnvDescriptor::Load:Cannot retrieve \"fGradientRadius\" for descriptor %s, setting default", DescriptorName );
	// Gradient Material
	std::string sGradientMaterial;
	if ( !Section->bGetString( "gradient_material", sGradientMaterial ) || ( !sGradientMaterial.length() ) ) {
		cMsgCRT( "cEnvDescriptor::Load:Invalid \"gradient_material\" in descriptor %s", DescriptorName );
		return false;
	}
	if ( sGradientMaterial != "none" ) {
		if ( !( pGradientMaterial = Leadwerks::Material::Load( "Materials\\Sky\\" + sGradientMaterial + ".mat" ) ) ) {
			cMsgCRT( "cEnvDescriptor::Load:Cannot load \"gradient_material\" in descriptor %s", DescriptorName );
			return false;
		}
	}



	// Sun Material
	std::string sSunMaterial;
	if ( !Section->bGetString( "sun_material", sSunMaterial ) || ( !sSunMaterial.length() ) ) {
		cMsgCRT( "cEnvDescriptor::Load:Invalid \"sun_material\" in descriptor %s", DescriptorName );
		return false;
	}
	if ( sSunMaterial != "none" ) {
		if ( !( pSunMaterial = Leadwerks::Material::Load( "Materials\\Sky\\" + sSunMaterial + ".mat" ) ) ) {
			cMsgCRT( "cEnvDescriptor::Load:Cannot load \"sun_material\" in descriptor %s", DescriptorName );
			return false;
		}
	}
	// Sun Color
	if ( !Section->bGetVec3( "sun_color", vSunColor, { 1.0f, 1.0f, 1.0f } ) )
		cMsgCRT( "cEnvDescriptor::Load:Cannot retrieve \"sun_color\" for descriptor %s, setting default", DescriptorName );
	// Sun Rotation
	if ( !Section->bGetVec3( "sun_rotation", vSunRotation, { -0.25f, -0.78f, 0.57f } ) )
		cMsgCRT( "cEnvDescriptor::Load:Cannot retrieve \"sun_rotation\" for descriptor %s, setting default", DescriptorName );
	
	return true;
}

bool cEnvDescriptor::LoadTexture() {
	if ( !( pSkyTexture = Leadwerks::Texture::Load( "Materials\\Sky\\" + sSkyTexture + ".tex" ) ) ) {
		cMsgCRT( "cEnvDescriptor::Load:Cannot LoadTexture \"sky_texture\" in descriptor %s", sIdentifier.c_str() );
		return false;
	}
	return true;
}

cEnvDescriptorMixer::cEnvDescriptorMixer( float Time ) : cEnvDescriptor( "Mixer", Time ) {}


cEnvDescriptorMixer *cEnvDescriptorMixer::Lerp( cEnvDescriptor *Current, cEnvDescriptor *Next, float Interpolant ) {
	
	// Ambient
	vAmbientColor		= Vec3Lerp( Current->vAmbientColor,		Next->vAmbientColor,	Interpolant );

	// Fog
	fFogFactor			= L_LERP( Current->fFogFactor,			Next->fFogFactor,		Interpolant );

	// SkyColor
	vSkyColor			= Vec3Lerp( Current->vSkyColor,			Next->vSkyColor,		Interpolant );

	// Gradient
	fGradientRadius		= L_LERP( Current->fGradientRadius,		Next->fGradientRadius,  Interpolant );

	// Sun
	vSunColor			= Vec3Lerp( Current->vSunColor,			Next->vSunColor,		Interpolant );
	vSunRotation		= Vec3Lerp( Current->vSunRotation,		Next->vSunRotation,		Interpolant );

	return this;
}