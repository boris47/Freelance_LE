

#pragma once


#include <Engine.h>		//
#include "Game\Entities\Item\Usables\FlashLight.h"

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_FlashLight( lua_State *L );

static int lua_FlashLight_GetRotation( lua_State *L ) {

	cFlashLight *pFlashLight = Engine()->ScriptEngine()->GetUserdata< cFlashLight >( 1 );
	Leadwerks::Interpreter::PushVec3( pFlashLight->GetRotation() );
	return 1;

}
static int lua_FlashLight_SetRotation( lua_State *L ) {

	cFlashLight *pFlashLight = Engine()->ScriptEngine()->GetUserdata< cFlashLight >( 1 );

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 2, "Vec3", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'SetRotation'.", &tolua_err );
		return 0;
	}

	Leadwerks::Vec3 vRotation = *( Leadwerks::Vec3* ) tolua_tousertype( L, 2, 0 );
	pFlashLight->SetRotation( vRotation );
	return 0;

}

static int lua_FlashLight_Enable( lua_State *L ) {

	cFlashLight *pFlashLight = Engine()->ScriptEngine()->GetUserdata< cFlashLight >( 1 );
	pFlashLight->Enable();
	return 0;

}
static int lua_FlashLight_Disable( lua_State *L ) {

	cFlashLight *pFlashLight = Engine()->ScriptEngine()->GetUserdata< cFlashLight >( 1 );
	pFlashLight->Disable();
	return 0;

}
static int lua_FlashLight_IsEnabled( lua_State *L ) {

	cFlashLight *pFlashLight = Engine()->ScriptEngine()->GetUserdata< cFlashLight >( 1 );
	lua_pushboolean( L, pFlashLight->IsEnabled() ? 1 : 0 );
	return 1;

}


static int lua_FlashLight_Activate( lua_State *L ) {

	cFlashLight *pFlashLight = Engine()->ScriptEngine()->GetUserdata< cFlashLight >( 1 );
	pFlashLight->Activate();
	return 0;

}
static int lua_FlashLight_Deactivate( lua_State *L ) {

	cFlashLight *pFlashLight = Engine()->ScriptEngine()->GetUserdata< cFlashLight >( 1 );
	pFlashLight->Deactivate();
	return 0;

}
static int lua_FlashLight_IsActive( lua_State *L ) {

	cFlashLight *pFlashLight = Engine()->ScriptEngine()->GetUserdata< cFlashLight >( 1 );
	lua_pushboolean( L, pFlashLight->IsActive() ? 1 : 0 );
	return 1;

}


bool LUA_Expose_FlashLight( lua_State *L ) {

	LUAMetaTable FlashLight = LUAMetaTable( L, "cFlashLight" );
	//				  Module	  Function name			Returned		Arguments
	LUA_CLASS_METHOD( FlashLight, GetRotation,			"Vec3",			"void" );
	LUA_CLASS_METHOD( FlashLight, SetRotation,			"void",			"Vec3" );
	LUA_CLASS_METHOD( FlashLight, Enable,				"void",			"void" );
	LUA_CLASS_METHOD( FlashLight, Disable,				"void",			"void" );
	LUA_CLASS_METHOD( FlashLight, IsEnabled,			"boolean",		"void" );

	LUA_CLASS_METHOD( FlashLight, Activate,				"void",			"void" );
	LUA_CLASS_METHOD( FlashLight, Deactivate,			"void",			"void" );
	LUA_CLASS_METHOD( FlashLight, IsActive,				"boolean",		"void" );

	if ( !FlashLight.Close() ) {
		cMsgCRT( "LUA_Expose_FlashLight: cFlashLight exposure fail" );
		return false;
	}
	return true;

}