

#pragma once


#include <Engine.h>		// Engine it self

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_Engine( lua_State *L );


static int lua_Engine_Call( lua_State *L ) {

	Engine()->ScriptEngine()->PushUserdata( Engine(), "cEngine" );
	return 1;

}

static int lua_Engine_AudioDevice( lua_State *L ) {

	Engine()->ScriptEngine()->PushUserdata( Engine()->AudioDevice(), "cAudioDevice" );
	return 1;

}

static int lua_Engine_DrawInfo( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	Engine()->DrawInfo( "[LUA] %s", lua_tostring( L, 2 ) );
	return 0;

}

static int lua_Engine_NewID( lua_State *L ) {

	lua_pushinteger( L, Engine()->NewID() );
	return 1;

}




bool LUA_Expose_Engine( lua_State *L ) {

	LUAClass Engine = LUAClass( L, "Engine", lua_Engine_Call );
	//				  Module  Function name			Returned		Arguments
	LUA_CLASS_METHOD( Engine, AudioDevice,			"cAudioDevice", "void" );
	LUA_CLASS_METHOD( Engine, DrawInfo,				"void",			"string" );
	LUA_CLASS_METHOD( Engine, NewID,				"int",			"void" );

	if ( !Engine.Close() ) {
		cMsgCRT( "LUA_Expose_Engine: cEngine exposure fail" );
		return false;
	}
	return true;
}