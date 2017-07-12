

#pragma once


#include <Engine.h>

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_Utils( lua_State *L );


static int lua_Utils_cMsgINFO( lua_State *L ) {

	luaL_checktype( L, 1, LUA_TSTRING );
	cMsgINFO( lua_tostring( L, 1 ) );
	return 0;

}

static int lua_Utils_cMsgDBG( lua_State *L ) {

	luaL_checktype( L, 1, LUA_TSTRING );
	cMsgDBG( lua_tostring( L, 1 ) );
	return 0;

}
static int lua_Utils_FileExists( lua_State *L ) {

	luaL_checktype( L, 1, LUA_TSTRING );
	lua_pushboolean( L, FileExists( lua_tostring( L, 1 ) ) ? 1 : 0 );
	return 1;

}

static int lua_Utils_GetAsEntity( lua_State *L ) {

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 1, "Entity", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'GetAsEntity'.", &tolua_err );
		return 0;
	}

	Leadwerks::Object *Entity = Leadwerks::Interpreter::ToObject( 1 );
	cEntity *pEntity = ( cEntity * ) Entity->GetUserData();

	Engine()->ScriptEngine()->PushUserdata( pEntity, "cEntity", true, true );
	return 1;

}

static int lua_Utils_Vec3Curve( lua_State *L ) {
	
	if ( lua_isnumber( L, 1 ) ) {	// Vec3Curve( float f, Leadwerks::Vec3 v, float d );

		tolua_Error tolua_err;
		if ( !tolua_isusertype( L, 2, "Vec3", 0, &tolua_err ) ) {
			tolua_error( L, "#ferror in function 'Vec3Curve'.", &tolua_err );
			return 0;
		}

		luaL_checktype( L, 3, LUA_TNUMBER );

		float fTarget = lua_tonumber( L, 1 );
		Leadwerks::Vec3 vCurrent = *( Leadwerks::Vec3* ) tolua_tousertype( L, 2, 0 );
		float fDivisions = lua_tonumber( L, 3 );

		Leadwerks::Interpreter::PushVec3( Vec3Curve( fTarget, vCurrent, fDivisions ) );
		return 1;
	}

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 1, "Vec3", 0, &tolua_err ) || !tolua_isusertype( L, 2, "Vec3", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'Vec3Curve'.", &tolua_err );
		return 0;
	}

	// Quindi deve essere Vec3Curve( Leadwerks::Vec3 v1, Leadwerks::Vec3 v2, float d );
	luaL_checktype( L, 3, LUA_TNUMBER );

	Leadwerks::Vec3 vCurrent = *( Leadwerks::Vec3* ) tolua_tousertype( L, 1, 0 );
	Leadwerks::Vec3 vTarget  = *( Leadwerks::Vec3* ) tolua_tousertype( L, 2, 0 );
	float fDivisions = lua_tonumber( L, 3 );

	Leadwerks::Interpreter::PushVec3( Vec3Curve( vTarget, vCurrent, fDivisions ) );
	return 1;

}

static int lua_Utils_Vec3Lerp( lua_State *L ) {

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 1, "Vec3", 0, &tolua_err ) || !tolua_isusertype( L, 2, "Vec3", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'Vec3Lerp'.", &tolua_err );
		return 0;
	}
	luaL_checktype( L, 3, LUA_TNUMBER );

	Leadwerks::Vec3 vCurrent = *( Leadwerks::Vec3* ) tolua_tousertype( L, 1, 0 );
	Leadwerks::Vec3 vTarget = *( Leadwerks::Vec3* ) tolua_tousertype( L, 2, 0 );
	float fInterpolant = lua_tonumber( L, 3 );

	Leadwerks::Interpreter::PushVec3( Vec3Lerp( vCurrent, vTarget, fInterpolant ) );
	return 1;

}



bool LUA_Expose_Utils( lua_State *L ) {

	LUANameSpace Utils = LUANameSpace( L, "Utils" );
	//				  Module Function name			Returned		Arguments
	LUA_CLASS_METHOD( Utils, cMsgINFO,				"void",			"string" );
	LUA_CLASS_METHOD( Utils, cMsgDBG,				"void",			"string" );
	LUA_CLASS_METHOD( Utils, FileExists,			"boolean",		"string" );
	LUA_CLASS_METHOD( Utils, GetAsEntity,			"Entity",		"cEntity" );
	LUA_CLASS_METHOD( Utils, Vec3Curve,				"Vec3",			"float or Vec3, Vec3, float" );
	LUA_CLASS_METHOD( Utils, Vec3Lerp,				"Vec3",			"Vec3, Vec3, float" );

	if ( !Utils.Close() ) {
		cMsgCRT( "LUA_Expose_Utils: Utils namespace exposure fail" );
		return false;
	}

	return true;

}