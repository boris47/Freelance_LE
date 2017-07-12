

#pragma once


#include <Engine.h>		// 

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_Camera_Effects( lua_State *L );


static int lua_HeadBob_Enable( lua_State *L ) {

	cHeadBob *pHeadBob = Engine()->ScriptEngine()->GetUserdata< cHeadBob >( 1 );
	pHeadBob->Enable();
	return 0;

}
static int lua_HeadBob_Disable( lua_State *L ) {

	cHeadBob *pHeadBob = Engine()->ScriptEngine()->GetUserdata< cHeadBob >( 1 );
	pHeadBob->Disable();
	return 0;

}
static int lua_HeadBob_IsActive( lua_State *L ) {

	cHeadBob *pHeadBob = Engine()->ScriptEngine()->GetUserdata< cHeadBob >( 1 );
	lua_pushboolean( L, pHeadBob->IsActive() ? 1 : 0 );
	return 1;

}
static int lua_HeadBob_GetAmplitudeMul( lua_State *L ) {

	cHeadBob *pHeadBob = Engine()->ScriptEngine()->GetUserdata< cHeadBob >( 1 );
	lua_pushnumber( L, pHeadBob->GetAmplitudeMul() );
	return 1;

}
static int lua_HeadBob_SetAmplitudeMul( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cHeadBob *pHeadBob = Engine()->ScriptEngine()->GetUserdata< cHeadBob >( 1 );
	pHeadBob->SetAmplitudeMul( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_HeadBob_GetSpeedMul( lua_State *L ) {

	cHeadBob *pHeadBob = Engine()->ScriptEngine()->GetUserdata< cHeadBob >( 1 );
	lua_pushnumber( L, pHeadBob->GetSpeedMul() );
	return 1;

}
static int lua_HeadBob_SetSpeedMul( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cHeadBob *pHeadBob = Engine()->ScriptEngine()->GetUserdata< cHeadBob >( 1 );
	pHeadBob->SetSpeedMul( lua_tonumber( L, 2 ) );
	return 0;

}



//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////




static int lua_HeadMove_Enable( lua_State *L ) {

	cHeadMove *pHeadMove = Engine()->ScriptEngine()->GetUserdata< cHeadMove >( 1 );
	pHeadMove->Enable();
	return 0;

}
static int lua_HeadMove_Disable( lua_State *L ) {

	cHeadMove *pHeadMove = Engine()->ScriptEngine()->GetUserdata< cHeadMove >( 1 );
	pHeadMove->Disable();
	return 0;

}
static int lua_HeadMove_IsActive( lua_State *L ) {

	cHeadMove *pHeadMove = Engine()->ScriptEngine()->GetUserdata< cHeadMove >( 1 );
	lua_pushboolean( L, pHeadMove->IsActive() ? 1 : 0 );
	return 1;

}
static int lua_HeadMove_GetAmplitudeMul( lua_State *L ) {

	cHeadMove *pHeadMove = Engine()->ScriptEngine()->GetUserdata< cHeadMove >( 1 );
	lua_pushnumber( L, pHeadMove->GetAmplitudeMul() );
	return 1;

}
static int lua_HeadMove_SetAmplitudeMul( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cHeadMove *pHeadMove = Engine()->ScriptEngine()->GetUserdata< cHeadMove >( 1 );
	pHeadMove->SetAmplitudeMul( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_HeadMove_GetSpeedMul( lua_State *L ) {

	cHeadMove *pHeadMove = Engine()->ScriptEngine()->GetUserdata< cHeadMove >( 1 );
	lua_pushnumber( L, pHeadMove->GetSpeedMul() );
	return 1;

}
static int lua_HeadMove_SetSpeedMul( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cHeadMove *pHeadMove = Engine()->ScriptEngine()->GetUserdata< cHeadMove >( 1 );
	pHeadMove->SetSpeedMul( lua_tonumber( L, 2 ) );
	return 0;

}



//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////




static int lua_HeadLean_Enable( lua_State *L ) {

	cHeadLean *pHeadLean = Engine()->ScriptEngine()->GetUserdata< cHeadLean >( 1 );
	pHeadLean->Enable();
	return 0;

}
static int lua_HeadLean_Disable( lua_State *L ) {

	cHeadLean *pHeadLean = Engine()->ScriptEngine()->GetUserdata< cHeadLean >( 1 );
	pHeadLean->Disable();
	return 0;

}
static int lua_HeadLean_IsActive( lua_State *L ) {

	cHeadMove *pHeadLean = Engine()->ScriptEngine()->GetUserdata< cHeadMove >( 1 );
	lua_pushboolean( L, pHeadLean->IsActive() ? 1 : 0 );
	return 1;

}
static int lua_HeadLean_GetAmplitudeMul( lua_State *L ) {

	cHeadLean *pHeadLean = Engine()->ScriptEngine()->GetUserdata< cHeadLean >( 1 );
	lua_pushnumber( L, pHeadLean->GetAmplitudeMul() );
	return 1;

}
static int lua_HeadLean_SetAmplitudeMul( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cHeadLean *pHeadLean = Engine()->ScriptEngine()->GetUserdata< cHeadLean >( 1 );
	pHeadLean->SetAmplitudeMul( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_HeadLean_GetSpeedMul( lua_State *L ) {

	cHeadLean *pHeadLean = Engine()->ScriptEngine()->GetUserdata< cHeadLean >( 1 );
	lua_pushnumber( L, pHeadLean->GetSpeedMul() );
	return 1;

}
static int lua_HeadLean_SetSpeedMul( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cHeadLean *pHeadLean = Engine()->ScriptEngine()->GetUserdata< cHeadLean >( 1 );
	pHeadLean->SetSpeedMul( lua_tonumber( L, 2 ) );
	return 0;

}



//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////




static int lua_Zoom_Enable( lua_State *L ) {

	cZoom *pZoom = Engine()->ScriptEngine()->GetUserdata< cZoom >( 1 );
	pZoom->Enable();
	return 0;

}
static int lua_Zoom_Disable( lua_State *L ) {

	cZoom *pZoom = Engine()->ScriptEngine()->GetUserdata< cZoom >( 1 );
	pZoom->Disable();
	return 0;

}
static int lua_Zoom_IsActive( lua_State *L ) {

	cZoom *pZoom = Engine()->ScriptEngine()->GetUserdata< cZoom >( 1 );
	lua_pushboolean( L, pZoom->IsActive() ? 1 : 0 );
	return 1;

}
static int lua_Zoom_IsZoomed( lua_State *L ) {

	cZoom *pZoom = Engine()->ScriptEngine()->GetUserdata< cZoom >( 1 );
	lua_pushboolean( L, pZoom->IsZoomed() ? 1 : 0 );
	return 1;

}
static int lua_Zoom_GetCamZoom( lua_State *L ) {

	cZoom *pZoom = Engine()->ScriptEngine()->GetUserdata< cZoom >( 1 );
	lua_pushnumber( L, pZoom->GetCamZoom() );
	return 1;

}




bool LUA_Expose_Camera_Effects( lua_State *L ) {

	LUAMetaTable HeadBob = LUAMetaTable( L, "cHeadBob" );
	//				  Module	Function name			Returned		Arguments
	LUA_CLASS_METHOD( HeadBob, Enable,					"void",			"void" );
	LUA_CLASS_METHOD( HeadBob, Disable,					"void",			"void" );
	LUA_CLASS_METHOD( HeadBob, IsActive,				"boolean",		"void" );
	LUA_CLASS_METHOD( HeadBob, GetAmplitudeMul,			"float",		"void" );
	LUA_CLASS_METHOD( HeadBob, SetAmplitudeMul,			"void",			"float" );
	LUA_CLASS_METHOD( HeadBob, GetSpeedMul,				"float",		"void" );
	LUA_CLASS_METHOD( HeadBob, SetSpeedMul,				"void",			"float" );

	if ( !HeadBob.Close() ) {
		cMsgCRT( "LUA_Expose_Camera_Effects: cHeadBob exposure fail" );
		return false;
	}

	LUAMetaTable HeadMove = LUAMetaTable( L, "cHeadMove" );
	//				  Module	Function name			Returned		Arguments
	LUA_CLASS_METHOD( HeadMove, Enable,					"void",			"void" );
	LUA_CLASS_METHOD( HeadMove, Disable,				"void",			"void" );
	LUA_CLASS_METHOD( HeadMove, IsActive,				"boolean",		"void" );
	LUA_CLASS_METHOD( HeadMove, GetAmplitudeMul,		"float",		"void" );
	LUA_CLASS_METHOD( HeadMove, SetAmplitudeMul,		"void",			"float" );
	LUA_CLASS_METHOD( HeadMove, GetSpeedMul,			"float",		"void" );
	LUA_CLASS_METHOD( HeadMove, SetSpeedMul,			"void",			"float" );

	if ( !HeadMove.Close() ) {
		cMsgCRT( "LUA_Expose_Camera_Effects: cHeadMove exposure fail" );
		return false;
	}

	LUAMetaTable HeadLean = LUAMetaTable( L, "cHeadLean" );
	//				  Module	Function name			Returned		Arguments
	LUA_CLASS_METHOD( HeadLean, Enable,					"void",			"void" );
	LUA_CLASS_METHOD( HeadLean, Disable,				"void",			"void" );
	LUA_CLASS_METHOD( HeadLean, IsActive,				"boolean",		"void" );
	LUA_CLASS_METHOD( HeadLean, GetAmplitudeMul,		"float",		"void" );
	LUA_CLASS_METHOD( HeadLean, SetAmplitudeMul,		"void",			"float" );
	LUA_CLASS_METHOD( HeadLean, GetSpeedMul,			"float",		"void" );
	LUA_CLASS_METHOD( HeadLean, SetSpeedMul,			"void",			"float" );

	if ( !HeadLean.Close() ) {
		cMsgCRT( "LUA_Expose_Camera_Effects: cHeadLean exposure fail" );
		return false;
	}

	LUAMetaTable Zoom = LUAMetaTable( L, "cZoom" );
	//				  Module	Function name			Returned		Arguments
	LUA_CLASS_METHOD( Zoom, Enable,						"void",			"void" );
	LUA_CLASS_METHOD( Zoom, Disable,					"void",			"void" );
	LUA_CLASS_METHOD( Zoom, IsActive,					"boolean",		"void" );
	LUA_CLASS_METHOD( Zoom, IsZoomed,					"boolean",		"void" );
	LUA_CLASS_METHOD( Zoom, GetCamZoom,					"float",		"void" );

	if ( !Zoom.Close() ) {
		cMsgCRT( "LUA_Expose_Camera_Effects: cZoom exposure fail" );
		return false;
	}
	return true;
}