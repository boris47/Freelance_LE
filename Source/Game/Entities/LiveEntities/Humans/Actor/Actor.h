
#pragma once


#include <Engine.h>
#include "InputManager.h"
#include "Game\Entities\LiveEntities\Humans\Human.h"

class cActor : public cHuman {
	
	private:

		// Internal Vars
		float fLastCamY = ZERO_FLOAT;
	
	public:
		cActor( Leadwerks::Vec3 Position, Leadwerks::Vec3 Direction );
		~cActor();

	private:
		void PhysicUpdate_Walk();
		void PhysicUpdate_Fly();
		void PhysicUpdate_Swim( bool bIsEntityInWater, bool bIsCameraUnderWater, bool bIsCameraReallyUnderWater );
		void PhysicUpdate_P1ToP2();

	public:
		
		void Update();
		void OnFrame();
};




