
#pragma once

#include "Game\Entities\Item\Item.h"

class cItemUsable : public cItem {

	public:
		cItemUsable();
		virtual ~cItemUsable();
		
		virtual void Update( void )		= 0;
		virtual void OnFrame( void )	= 0;
};