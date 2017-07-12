

#pragma once


#include <Engine.h>		//
#include "Game\Level\Level.h"
#include "Game\Entities\LiveEntities\Humans\Actor\Actor.h"

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_Actor( lua_State *L );


bool LUA_Expose_Actor( lua_State *L ) {

	// Returned by Level
	LUAMetaTable Actor = LUAMetaTable( L, "cActor" );

	if ( !Actor.Close() ) {
		cMsgCRT( "LUA_Expose_Actor: cActor exposure fail" );
		return false;
	}

	return true;

}
