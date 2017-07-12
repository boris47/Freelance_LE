
#pragma once


#include <Engine.h>
#include "Game\InputManager.h"
#include "Game\Entities\LiveEntities\Humans\Human.h"

class cActor : public cHuman {
	
	private:

		cInputManager InputManager;
	
	public:
		cActor( Leadwerks::Vec3 Position, Leadwerks::Vec3 Direction, Leadwerks::Vec3 Scale );
		~cActor();

	public:
		
		void PhysicUpdate( void );
		void OnFrame( void );
};




