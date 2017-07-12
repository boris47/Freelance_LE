
#pragma once

#include <Engine.h>
#include "EffectBase.h"

// Forward declaration
class cEntity;

class cHeadMove : public cEffectBase {
	private:
		Leadwerks::Vec3 vRotation	= EMPTY_VEC3;

		Leadwerks::Vec3 vTargetRot	= EMPTY_VEC3;

		// Values read from config file
		float fCFG_Amplitude, fCFG_Speed, fTheta_Update_Vert, fTheta_Update_Oriz;

		// Internal Vars
		float fThetaX				= ZERO_FLOAT;
		float fThetaY				= ZERO_FLOAT;

	public:

		cHeadMove( cSection *CameraSection );
		
		inline Leadwerks::Vec3	GetRotation() { return vRotation; }

		void					Update( cEntity *CameraParent );

		void					Reset( bool bInstant = false );

};
