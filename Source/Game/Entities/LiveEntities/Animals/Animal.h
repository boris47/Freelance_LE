
#pragma once

#include <Engine.h>
#include "Game\Entities\LiveEntities\LiveEntity.h"

class cAnimal : public cLiveEntity {
	private:
		

	public:
		virtual void Update( void )		= 0;
		virtual void OnFrame( void )	= 0;
		

};