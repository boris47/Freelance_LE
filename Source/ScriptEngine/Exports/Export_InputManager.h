

#pragma once


#include <Engine.h>		// cInputManager

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_InputManager( lua_State *L );

static int lua_InputManager_Forward( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->Forward() ? 1 : 0 );
	return 1;

}
static int lua_InputManager_Backward( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->Backward() ? 1 : 0 );
	return 1;

}

static int lua_InputManager_StrafeLeft( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->StrafeLeft() ? 1 : 0 );
	return 1;

}
static int lua_InputManager_StrafeRight( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->StrafeRight() ? 1 : 0 );
	return 1;

}

static int lua_InputManager_LeanLeft( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->LeanLeft() ? 1 : 0 );
	return 1;

}
static int lua_InputManager_LeanRight( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->LeanRight() ? 1 : 0 );
	return 1;

}

static int lua_InputManager_Crouch( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->Crouch() ? 1 : 0 );
	return 1;

}

static int lua_InputManager_Jump( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->Jump() ? 1 : 0 );
	return 1;

}

static int lua_InputManager_Run( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->Run() ? 1 : 0 );
	return 1;

}

static int lua_InputManager_Use( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->Use() ? 1 : 0 );
	return 1;

}

static int lua_InputManager_Item1( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->Item1() ? 1 : 0 );
	return 1;

}
static int lua_InputManager_Item2( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->Item2() ? 1 : 0 );
	return 1;

}
static int lua_InputManager_Item3( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->Item3() ? 1 : 0 );
	return 1;

}
static int lua_InputManager_Item4( lua_State *L ) {

	lua_pushboolean( L, Engine()->InputManager()->Item4() ? 1 : 0 );
	return 1;

}



bool LUA_Expose_InputManager( lua_State *L ) {

	LUANameSpace InputManager = LUANameSpace( L, "InputManager" );
	//				  Module		Function name			Returned	Arguments
	LUA_CLASS_METHOD( InputManager, Forward,				"bool",		"void" );
	LUA_CLASS_METHOD( InputManager, Backward,				"bool",		"void" );
	LUA_CLASS_METHOD( InputManager, StrafeLeft,				"bool",		"void" );
	LUA_CLASS_METHOD( InputManager, StrafeRight,			"bool",		"void" );

	LUA_CLASS_METHOD( InputManager, LeanLeft,				"bool",		"void" );
	LUA_CLASS_METHOD( InputManager, LeanRight,				"bool",		"void" );

	LUA_CLASS_METHOD( InputManager, Crouch,					"bool",		"void" );
	LUA_CLASS_METHOD( InputManager, Jump,					"bool",		"void" );
	LUA_CLASS_METHOD( InputManager, Run,					"bool",		"void" );

	LUA_CLASS_METHOD( InputManager, Use,					"bool",		"void" );

	LUA_CLASS_METHOD( InputManager, Item1,					"bool",		"void" );
	LUA_CLASS_METHOD( InputManager, Item2,					"bool",		"void" );
	LUA_CLASS_METHOD( InputManager, Item3,					"bool",		"void" );
	LUA_CLASS_METHOD( InputManager, Item4,					"bool",		"void" );

	if ( !InputManager.Close() ) {
		cMsgCRT( "LUA_Expose_InputManager: cInputManager exposure fail" );
		return false;
	}
	return true;
}