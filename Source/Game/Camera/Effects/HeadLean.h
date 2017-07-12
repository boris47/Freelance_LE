
#pragma once

#include <Engine.h>
#include "EffectBase.h"

// Forward declaration
class cEntity;

class cHeadLean : public cEffectBase {
	private:

		// Lean Ability
		Leadwerks::Vec3 vPosition	= EMPTY_VEC3;

		Leadwerks::Vec3 vRotation	= EMPTY_VEC3;

		// Value read from config file
		float fCFG_Lean_Max;


	public:
		cHeadLean( cSection *CameraSection );
		
		inline Leadwerks::Vec3	GetPosition()					{ return vPosition; }
		inline Leadwerks::Vec3	GetRotation()					{ return vRotation; }
		
		void					Update( cEntity *CameraParent );

		void					Reset( bool bInstant = false );


};
