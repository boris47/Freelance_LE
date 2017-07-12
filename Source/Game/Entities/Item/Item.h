
#pragma once

#include <Engine.h>
#include "Game\Entities\Entity.h"

class cItem : public cEntity {
	private:
		
	public:
		cItem();
		virtual ~cItem();

		virtual void Update( void )		= 0;
		virtual void OnFrame( void )	= 0;
};