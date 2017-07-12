

#pragma once


#include <Engine.h>		//
#include "Game\Entities\Entity.h"

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_Entity( lua_State *L );

static int lua_Entity_IsHuman( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1, true );
	lua_pushboolean( L, ( pEntity->IsHuman() ) ? 1 : 0 );
	return 1;

}

static int lua_Entity_IsLiveEntity( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1, true );
	lua_pushboolean( L, ( pEntity->IsLiveEntity() ) ? 1 : 0 );
	return 1;

}

static int lua_Entity_ID( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	lua_pushinteger( L, pEntity->ID() );
	return 1;

}

static int lua_Entity_Name( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	lua_pushstring( L, pEntity->Name().c_str() );
	return 1;

}

static int lua_Entity_Section( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	lua_pushstring( L, pEntity->Section().c_str() );
	return 1;

}

static int lua_Entity_SetParent( lua_State *L ) {

	Leadwerks::Entity *pParent = GET_AS( Leadwerks::Interpreter::ToObject( 2 ), Leadwerks::Entity );
	if ( !pParent )	return luaL_typerror( L, 2, "Leadwerks::Entity" );

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	pEntity->SetParent( pParent );
	return 0;

}

static int lua_Entity_GetParent( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	tolua_pushusertype( L, pEntity->GetParent(), "Entity" );
	return 1;

}

static int lua_Entity_GetEntity( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	tolua_pushusertype( L, pEntity->GetEntity(), "Entity" );
	return 1;

}

static int lua_Entity_Stop( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	pEntity->Stop();
	return 0;

}

static int lua_Entity_GetType( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	lua_pushnumber( L, pEntity->GetType() );
	return 1;

}

/*
static int lua_Entity_SetRotation( lua_State *L ) {

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 2, "Vec3", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'SetRotation'.", &tolua_err );
		return 0;
	}

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	Leadwerks::Vec3 vRotation = *( Leadwerks::Vec3* ) tolua_tousertype( L, 2, 0 );
	pEntity->SetRotation( vRotation );
	return 0;

}

static int lua_Entity_GetRotation( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	Leadwerks::Interpreter::PushVec3( pEntity->GetRotation() );
	return 1;

}
*/
static int lua_Entity_SetRotationP1ToP2( lua_State *L ) {

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 2, "Vec3", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'SetRotationP1ToP2'.", &tolua_err );
		return 0;
	}

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	Leadwerks::Vec3 vRotation = *( Leadwerks::Vec3* ) tolua_tousertype( L, 2, 0 );
	pEntity->SetRotationP1ToP2( vRotation );
	return 0;

}

static int lua_Entity_GetRotationP1ToP2( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	Leadwerks::Interpreter::PushVec3( pEntity->GetRotationP1ToP2() );
	return 1;
}
/*
static int lua_Entity_SetPosition( lua_State *L ) {

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 2, "Vec3", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'SetPosition'.", &tolua_err );
		return 0;
	}

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	Leadwerks::Vec3 vPosition = *( Leadwerks::Vec3* ) tolua_tousertype( L, 2, 0 );
	pEntity->SetPosition( vPosition );
	return 0;

}

static int lua_Entity_GetPosition( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	Leadwerks::Interpreter::PushVec3( pEntity->GetPosition() );
	return 1;
}

static int lua_Entity_SetScale( lua_State *L ) {

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 2, "Vec3", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'SetScale'.", &tolua_err );
		return 0;
	}

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	Leadwerks::Vec3 vPosition = *( Leadwerks::Vec3* ) tolua_tousertype( L, 2, 0 );
	pEntity->SetScale( vPosition );
	return 0;

}

static int lua_Entity_GetScale( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1 );
	Leadwerks::Interpreter::PushVec3( pEntity->GetScale() );
	return 1;
}
*/

bool LUA_Expose_Entity( lua_State *L ) {

	LUAMetaTable Entity = LUAMetaTable( L, "cEntity" );
	//				  Module  Function name			Returned		Arguments
	LUA_CLASS_METHOD( Entity, IsHuman,				"boolean",		"void" );
	LUA_CLASS_METHOD( Entity, IsLiveEntity,			"boolean",		"void" );

	LUA_CLASS_METHOD( Entity, ID,					"int",			"void" );
	LUA_CLASS_METHOD( Entity, Name,					"string",		"void" );
	LUA_CLASS_METHOD( Entity, Section,				"string",		"void" );
	LUA_CLASS_METHOD( Entity, SetParent,			"void",			"cEntity" );
	LUA_CLASS_METHOD( Entity, GetParent,			"cEntity",		"void" );
	LUA_CLASS_METHOD( Entity, GetEntity,			"Entity",		"void" );
	LUA_CLASS_METHOD( Entity, Stop,					"void",			"void" );
	LUA_CLASS_METHOD( Entity, GetType,				"int",			"void" );
//	LUA_CLASS_METHOD( Entity, SetRotation,			"void",			"Vec3" );
//	LUA_CLASS_METHOD( Entity, GetRotation,			"Vec3",			"void" );
	LUA_CLASS_METHOD( Entity, SetRotationP1ToP2,	"void",			"Vec3" );
	LUA_CLASS_METHOD( Entity, GetRotationP1ToP2,	"Vec3",			"void" );
//	LUA_CLASS_METHOD( Entity, SetPosition,			"void",			"Vec3" );
//	LUA_CLASS_METHOD( Entity, GetPosition,			"Vec3",			"void" );
//	LUA_CLASS_METHOD( Entity, SetScale,				"void",			"Vec3" );
//	LUA_CLASS_METHOD( Entity, GetScale,				"Vec3",			"void" );

	if ( !Entity.Close() ) {
		cMsgCRT( "LUA_Expose_Entity: cEntity exposure fail" );
		return false;
	}

	return true;
}