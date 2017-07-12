
#pragma once

#include <Engine.h>
#include "EffectBase.h"

// Forward declaration
class cEntity;

class cHeadBob : public cEffectBase {
	private:
		// Headbob Effect
		Leadwerks::Vec3 vPosition	= EMPTY_VEC3;

		Leadwerks::Vec3 vTargetPos	= EMPTY_VEC3;

		Leadwerks::Vec3 vRotation	= EMPTY_VEC3;

		Leadwerks::Vec3 vTargetDir	= EMPTY_VEC3;

		float fThetaX				= ZERO_FLOAT;
		float fThetaY				= ZERO_FLOAT;

		float fStepValue			= ZERO_FLOAT;
		bool bStepDone				= false;

		// Values read from config file
		float fCFG_Amplitude, fCFG_Speed, fCFG_Step_Value, fTheta_Update;

	public:
		
		cHeadBob( cSection *CameraSection );
		
		inline Leadwerks::Vec3	GetPosition() { return vPosition; }
		inline Leadwerks::Vec3	GetRotation() { return vRotation; }
		
		void					Update( cEntity *CameraParent );

		void					Reset( bool bInstant = false );

};
