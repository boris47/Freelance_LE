

#pragma once


#include <Engine.h>		//
#include "Game\Entities\LiveEntities\Inventory.h"

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_Inventory( lua_State *L );









bool LUA_Expose_Inventory( lua_State *L ) {

	LUAMetaTable Inventory = LUAMetaTable( L, "cInventory" );

	if ( !Inventory.Close() ) {
		cMsgCRT( "LUA_Expose_Inventory: cInventory exposure fail" );
		return false;
	}
	return true;

}