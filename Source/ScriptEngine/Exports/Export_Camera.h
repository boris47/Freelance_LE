

#pragma once


#include <Engine.h>
#include "Game\Camera\CamManager.h"
#include "Game\Entities\Entity.h"

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_CamManager( lua_State *L );


static int lua_CamManager_Call( lua_State *L ) {
	Engine()->ScriptEngine()->PushUserdata( CamManager(), "cCamManager" );
	return 1;
}


static int lua_CamManager_IsRotating( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	lua_pushboolean( L, ( pCamManager->IsRotating() ) ? 1 : 0 );
	return 1;

}

static int lua_CamManager_SetParent( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TUSERDATA );
	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 2 );
	lua_pushboolean( L, ( pCamManager->SetParent( pEntity ) ) ? 1 : 0 );
	return 1;

}

static int lua_CamManager_GetParent( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	Engine()->ScriptEngine()->PushUserdata( pCamManager->GetParent(), "cEntity" );
	return 1;

}

static int lua_CamManager_GetCamera( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	tolua_pushusertype( L, pCamManager->GetCamera(), "Camera" );
	return 1;

}

static int lua_CamManager_GetStdHeight( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	lua_pushnumber( L, pCamManager->GetStdHeight() );
	return 1;

}

static int lua_CamManager_GetPointedDistance( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	lua_pushnumber( L, pCamManager->GetPointedDistance() );
	return 1;

}

static int lua_CamManager_SetCameraFirstPerson( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	pCamManager->SetCameraFirstPerson();
	return 0;

}

static int lua_CamManager_SetCameraThirdPerson( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	pCamManager->SetCameraThirdPerson();
	return 0;

}

static int lua_CamManager_SetCameraFree( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	pCamManager->SetCameraFree();
	return 0;

}

static int lua_CamManager_IsCameraAttached( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	lua_pushboolean( L, ( pCamManager->IsCameraAttached() ) ? 1 : 0 );
	return 1;

}

static int lua_CamManager_IsCameraFree( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	lua_pushboolean( L, ( pCamManager->IsCameraFree() ) ? 1 : 0 );
	return 1;

}

static int lua_CamManager_GetHeadBobEffect( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	Engine()->ScriptEngine()->PushUserdata( pCamManager->GetHeadBobEffect(), "cHeadBob" );
	return 1;

}

static int lua_CamManager_GetHeadMoveEffect( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	Engine()->ScriptEngine()->PushUserdata( pCamManager->GetHeadMoveEffect(), "cHeadMove" );
	return 1;

}

static int lua_CamManager_GetHeadLeanEffect( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	Engine()->ScriptEngine()->PushUserdata( pCamManager->GetHeadLeanEffect(), "cHeadLean" );
	return 1;
}

static int lua_CamManager_GetZoomEffect( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	Engine()->ScriptEngine()->PushUserdata( pCamManager->GetZoomEffect(), "cZoom" );
	return 1;
}

static int lua_CamManager_Detach( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	pCamManager->Detach();
	return 0;

}
static int lua_CamManager_Attach( lua_State *L ) {

	cCamManager *pCamManager = Engine()->ScriptEngine()->GetUserdata< cCamManager >( 1 );
	pCamManager->Attach();
	return 0;

}

bool LUA_Expose_CamManager( lua_State *L ) {

	LUAClass CamManager = LUAClass( L, "CamManager", lua_CamManager_Call );
	//				  Module	  Function name			Returned		Arguments
	LUA_CLASS_METHOD( CamManager, IsRotating,			"boolean",		"void" );
	LUA_CLASS_METHOD( CamManager, SetParent,			"boolean",		"cEntity" );
	LUA_CLASS_METHOD( CamManager, GetParent,			"cEntity",		"void" );
	LUA_CLASS_METHOD( CamManager, GetCamera,			"Camera",		"void" );
	LUA_CLASS_METHOD( CamManager, GetStdHeight,			"int",			"void" );
	LUA_CLASS_METHOD( CamManager, GetPointedDistance,	"float",		"void" );
	LUA_CLASS_METHOD( CamManager, SetCameraFirstPerson,	"void",			"void" );
	LUA_CLASS_METHOD( CamManager, SetCameraThirdPerson,	"void",			"void" );
	LUA_CLASS_METHOD( CamManager, SetCameraFree,		"void",			"void" );
	LUA_CLASS_METHOD( CamManager, IsCameraAttached,		"bolean",		"void" );
	LUA_CLASS_METHOD( CamManager, IsCameraFree,			"bolean",		"void" );
	LUA_CLASS_METHOD( CamManager, GetHeadBobEffect,		"cHeadBob",		"void" );
	LUA_CLASS_METHOD( CamManager, GetHeadMoveEffect,	"cHeadMove",	"void" );
	LUA_CLASS_METHOD( CamManager, GetHeadLeanEffect,	"cHeadLean",	"void" );
	LUA_CLASS_METHOD( CamManager, GetZoomEffect,		"cZoom",		"void" );
	LUA_CLASS_METHOD( CamManager, Detach,				"void",			"void" );
	LUA_CLASS_METHOD( CamManager, Attach, "void", "void" );

	if ( !CamManager.Close() ) {
		cMsgCRT( "LUA_Expose_CamManager: cCamManager exposure fail" );
		return false;
	}

	return true;
}

