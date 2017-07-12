

#pragma once


#include <Engine.h>		//
#include "Game\Entities\LiveEntities\Humans\Human.h"

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_Human( lua_State *L );

static int lua_Human_IsDragging( lua_State *L ) {

	cHuman *pHuman = Engine()->ScriptEngine()->GetUserdata< cHuman >( 1 );
	lua_pushboolean( L, pHuman->IsDragging() ? 1 : 0 );
	return 1;

}

static int lua_Human_SetDragging( lua_State *L ) {
	
	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 2, "Entity", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'SetDragging'.", &tolua_err );
		return 0;
	}

	Leadwerks::Entity *pEntity = GET_AS( Leadwerks::Interpreter::ToObject( 2 ), Leadwerks::Entity );
	if ( !pEntity )	return luaL_typerror( L, 2, "Leadwerks::Entity" );

	cHuman *pHuman = Engine()->ScriptEngine()->GetUserdata< cHuman >( 1 );
	pHuman->SetDragging( pEntity );
	return 0;

}

static int lua_Human_GetDraggedEntity( lua_State *L ) {

	cHuman *pHuman = Engine()->ScriptEngine()->GetUserdata< cHuman >( 1 );
	if ( pHuman->IsDragging() )
		tolua_pushusertype( L, pHuman->GetEntityDragged(), "Entity" );
	else {
		lua_pushnil( L );
	}
	return 1;

}

static int lua_Human_DropEntityDragged( lua_State *L ) {

	cHuman *pHuman = Engine()->ScriptEngine()->GetUserdata< cHuman >( 1 );
	pHuman->DropEntityDragged();
	return 0;

}

bool LUA_Expose_Human( lua_State *L ) {

	// Returned by Level
	LUAMetaTable Human = LUAMetaTable( L, "cHuman" );
	//				  Module Function name			Returned		Arguments
	LUA_CLASS_METHOD( Human, IsDragging,			"boolean",		"void" );
	LUA_CLASS_METHOD( Human, SetDragging,			"void",			"boolean" );
	LUA_CLASS_METHOD( Human, GetDraggedEntity,		"Entity",		"void" );
	LUA_CLASS_METHOD( Human, DropEntityDragged,		"void",			"void" );

	if ( !Human.Close() ) {
		cMsgCRT( "LUA_Expose_Human: cHuman exposure fail" );
		return false;
	}

	return true;

}