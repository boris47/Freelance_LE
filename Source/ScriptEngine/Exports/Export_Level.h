

#pragma once


#include <Engine.h>		//
#include "Game\Level\Level.h"

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_Level( lua_State *L );

static int lua_Level_Call( lua_State *L ) {

	if ( !Engine()->ScriptEngine()->PushUserdata( Level(), "cLevel" ) ) {	// Userdata or nil
		return luaL_error( L, "[ LUA ]: cLevel |Push| Pushing data fail" );	// 0
	}
	return 1;

}

static int lua_Level_Name( lua_State *L ) {

	lua_pushstring( L, Level()->Name().c_str() );
	return 1;

}

static int lua_Level_GetActor( lua_State *L ) {

	if ( !Engine()->ScriptEngine()->PushUserdata( Level()->GetActor(), "cActor" ) ) {	// push userdata or nil
		return luaL_error( L, "[ LUA ]: cLevel:GetActor |Push| Pushing data fail" );	// 0
	}
	return 1;

}

static int lua_Level_GetTimeFactor( lua_State *L ) {

	lua_pushnumber( L, Level()->GetTimeFactor() );
	return 1;

}
static int lua_Level_SetTimeFactor( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	Level()->SetTimeFactor( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_Level_NewID( lua_State *L ) {

	lua_pushnumber( L, Level()->NewID() );
	return 1;

}

static int lua_Level_Spawn( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 3, "Vec3", 0, &tolua_err ) || !tolua_isusertype( L, 4, "Vec3", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'Spawn'.", &tolua_err );
		return 0;
	}

	Leadwerks::Vec3 vPosition = *( Leadwerks::Vec3* ) tolua_tousertype( L, 3, 0 );
	Leadwerks::Vec3 vRotation = *( Leadwerks::Vec3* ) tolua_tousertype( L, 4, 0 );

	Leadwerks::Vec3 vScale = { 1, 1, 1 };
	if ( tolua_isusertype( L, 5, "Vec3", 0, NULL ) ) {
		vScale = *( Leadwerks::Vec3* ) tolua_tousertype( L, 5, 0 );
	}

	cEntity *pEntity = Level()->Spawn( lua_tostring( L, 2 ), vPosition, vRotation, vScale );
	Engine()->ScriptEngine()->PushUserdata( pEntity, "cEntity" );
	return 1;

}

static int lua_Level_FindByID( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	Engine()->ScriptEngine()->PushUserdata( Level()->FindByID( lua_tonumber( L, 2 ) ), "cEntity" );
	return 1;

}
static int lua_Level_FindByName( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	Engine()->ScriptEngine()->PushUserdata( Level()->FindByName( lua_tostring( L, 2 ) ), "cEntity" );
	return 1;

}
static int lua_Level_Release( lua_State *L ) { 
	
	luaL_checktype( L, 2, LUA_TNUMBER );
	Level()->Release( lua_tointeger( L, 2 ) );
	return 0;

}

static int lua_Level_WeatherManager( lua_State *L ) {

	Engine()->ScriptEngine()->PushUserdata( Level()->WeatherManager(), "cWeatherManager" );
	return 1;

}


bool LUA_Expose_Level( lua_State *L ) {

	LUAClass Level = LUAClass( L, "Level", lua_Level_Call );
	//				  Module Function name			Returned						Arguments
	LUA_CLASS_METHOD( Level, Name,					"string",						"void" );
	LUA_CLASS_METHOD( Level, GetActor,				"cActor",						"void" );
	LUA_CLASS_METHOD( Level, GetTimeFactor,			"float",						"void" );
	LUA_CLASS_METHOD( Level, SetTimeFactor,			"void",							"float" );
	LUA_CLASS_METHOD( Level, NewID,					"int",							"void" );
	LUA_CLASS_METHOD( Level, Spawn,					"string, Vec3, Vec3, (Vec3)",	"cEntity" );
	LUA_CLASS_METHOD( Level, FindByID,				"cEntity",						"int" );
	LUA_CLASS_METHOD( Level, FindByName,			"cEntity",						"string" );
	LUA_CLASS_METHOD( Level, Release,				"void",							"int" );
	LUA_CLASS_METHOD( Level, WeatherManager,		"cWeatherManager",				"void" );

	if ( !Level.Close() ) {
		cMsgCRT( "LUA_Expose_Level: cLevel exposure fail" );
		return false;
	}
	return true;
}