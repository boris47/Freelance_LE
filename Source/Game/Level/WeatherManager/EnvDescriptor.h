
#pragma once

#include <Engine.h>

struct cEnvDescriptor {

		float					fExecTime			= ZERO_FLOAT;
		std::string				sIdentifier			= "";
		cEnvDescriptor *		pNext				= NULL;

		// Ambient Color
		Leadwerks::Vec3			vAmbientColor		= EMPTY_VEC3;
		float					fFogFactor			= ZERO_FLOAT;

		// Sky
		std::string				sSkyTexture			= "";
		Leadwerks::Texture *	pSkyTexture			= NULL;
		Leadwerks::Vec3			vSkyColor			= EMPTY_VEC3;

		// Gradient
		float					fGradientRadius		= 0.5f;
		Leadwerks::Material *	pGradientMaterial	= NULL;

		// Sun
		Leadwerks::Vec3			vSunColor			= EMPTY_VEC3;
		Leadwerks::Vec3			vSunRotation		= EMPTY_VEC3;
		Leadwerks::Material *	pSunMaterial		= NULL;

		cEnvDescriptor( std::string Identifier, float Time );
		~cEnvDescriptor();
		
		bool Load( cSection *Section );
		bool LoadTexture();

};


struct cEnvDescriptorMixer : public cEnvDescriptor {

		cEnvDescriptorMixer( float Time );
		cEnvDescriptorMixer *Lerp( cEnvDescriptor *A, cEnvDescriptor *B, float Interpolant );

};