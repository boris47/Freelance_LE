

#pragma once


#include <Engine.h>		//
#include "Game\Level\Level.h"
#include "Game\Level\WeatherManager\WeatherManager.h"

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_WeatherManager( lua_State *L );

static int lua_WeatherManager_SetDynamic( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TBOOLEAN );
	cWeatherManager *pWeatherManager = Engine()->ScriptEngine()->GetUserdata< cWeatherManager >( 1 );
	pWeatherManager->SetDynamic( lua_toboolean( L, 2 ) ? true : false );
	return 0;

}

static int lua_WeatherManager_SetTime( lua_State *L ) {

	luaL_checkany( L, 2 );
	if ( !lua_isnumber( L, 2 ) && !lua_isstring( L, 2 ) )
		return luaL_typerror( L, 2, "number or string(Es 00:00:00)" );

	cWeatherManager *pWeatherManager = Engine()->ScriptEngine()->GetUserdata< cWeatherManager >( 1 );
	if ( lua_isnumber( L, 2 ) ) {

		int iH = lua_tonumber( L, 2 );
		int iM = lua_tonumber( L, 3 ); // Or 0
		int iS = lua_tonumber( L, 4 ); // Or 0

		pWeatherManager->SetTime( iH, iM, iS );
		return 0;
	}

	// So is a string
	pWeatherManager->SetTime( lua_tostring( L, 2 ) );
	return 0;
}


static int lua_WeatherManager_GetTime( lua_State *L ) {

	cWeatherManager *pWeatherManager = Engine()->ScriptEngine()->GetUserdata< cWeatherManager >( 1 );
	lua_pushnumber( L, pWeatherManager->GetTime() );
	return 1;

}

static int lua_WeatherManager_GetTimeAsString( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cWeatherManager *pWeatherManager = Engine()->ScriptEngine()->GetUserdata< cWeatherManager >( 1 );
	lua_pushstring( L, pWeatherManager->GetTimeAsString( lua_tonumber( L, 2 ) ).c_str() );
	return 1;

}

static int lua_WeatherManager_TansformTime( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cWeatherManager *pWeatherManager = Engine()->ScriptEngine()->GetUserdata< cWeatherManager >( 1 );
	float fTime = 0.0f;
	bool bRes = pWeatherManager->TansformTime( lua_tostring( L, 2 ), fTime );
	lua_pushnumber( L, fTime );
	lua_pushboolean( L, bRes ? 1 : 0 );
	return 2;

}

static int lua_WeatherManager_GetSun( lua_State *L ) {

	cWeatherManager *pWeatherManager = Engine()->ScriptEngine()->GetUserdata< cWeatherManager >( 1 );
	tolua_pushusertype( L, pWeatherManager->GetSun(), "DirectionalLight" );
	return 1;

}

static int lua_WeatherManager_SetSunShader( lua_State *L ) {

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 2, "Shader", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'SetSunShader'.", &tolua_err );
		return 0;
	}

	cWeatherManager *pWeatherManager = Engine()->ScriptEngine()->GetUserdata< cWeatherManager >( 1 );
	Leadwerks::Shader *p = ( Leadwerks::Shader* ) tolua_tousertype( L, 2, 0  );
	pWeatherManager->SetSunShader( p );
	return 0;

}

static int lua_WeatherManager_SetFogShader( lua_State *L ) {

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 2, "Shader", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'SetFogShader'.", &tolua_err );
		return 0;
	}

	cWeatherManager *pWeatherManager = Engine()->ScriptEngine()->GetUserdata< cWeatherManager >( 1 );
	Leadwerks::Shader *p = ( Leadwerks::Shader* ) tolua_tousertype( L, 2, 0 );
	pWeatherManager->SetFogShader( p );
	return 0;

}

static int lua_WeatherManager_GetAmbientColor( lua_State *L ) {

	cWeatherManager *pWeatherManager = Engine()->ScriptEngine()->GetUserdata< cWeatherManager >( 1 );
	Leadwerks::Interpreter::PushVec3( pWeatherManager->GetAmbientColor() );
	return 1;

}


bool LUA_Expose_WeatherManager( lua_State *L ) {

	LUAMetaTable WeatherManager = LUAMetaTable( L, "cWeatherManager" );
	//				  Module Function name				Returned			Arguments
	LUA_CLASS_METHOD( WeatherManager, SetDynamic,		"void",				"bool" );

	LUA_CLASS_METHOD( WeatherManager, SetTime,			"void",				"float or string" );
	LUA_CLASS_METHOD( WeatherManager, GetTime,			"float",			"void" );
	LUA_CLASS_METHOD( WeatherManager, GetTimeAsString,	"string",			"float" );
	LUA_CLASS_METHOD( WeatherManager, TansformTime,		"float, boolean",	"string" );

	LUA_CLASS_METHOD( WeatherManager, GetSun,			"DirectionalLight",	"void" );

	LUA_CLASS_METHOD( WeatherManager, SetSunShader,		"void",				"Shader" );
	LUA_CLASS_METHOD( WeatherManager, SetFogShader,		"void",				"Shader" );

	LUA_CLASS_METHOD( WeatherManager, GetAmbientColor,	"Vec3",				"void" );

	if ( !WeatherManager.Close() ) {
		cMsgCRT( "LUA_Expose_WeatherManager: cWeatherManager exposure fail" );
		return false;
	}
	return true;

}