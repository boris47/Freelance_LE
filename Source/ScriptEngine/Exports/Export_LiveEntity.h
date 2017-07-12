

#pragma once


#include <Engine.h>		//
#include "Game\Entities\LiveEntities\LiveEntity.h"

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_LiveEntity( lua_State *L );

static int lua_LiveEntity_IsHuman( lua_State *L ) {

	cEntity *pEntity = Engine()->ScriptEngine()->GetUserdata< cEntity >( 1, true );
	lua_pushboolean( L, ( pEntity->IsHuman() ) ? 1 : 0 );
	return 1;

}

static int lua_LiveEntity_SetHealth( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetHealth( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_LiveEntity_GetHealth( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushnumber( L, pLiveEntity->GetHealth() );
	return 1;

}

static int lua_LiveEntity_SetStamina( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetStamina( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_LiveEntity_GetStamina( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushnumber( L, pLiveEntity->GetStamina() );
	return 1;

}

static int lua_LiveEntity_SetViewRange( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetViewRange( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_LiveEntity_GetViewRange( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushnumber( L, pLiveEntity->GetViewRange() );
	return 1;

}

static int lua_LiveEntity_SetMotionType( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	int iValue = lua_tonumber( L, 2 );
	if ( ( iValue > -1 ) && ( iValue < 3 ) )
		pLiveEntity->SetMotionType( iValue );
	return 0;

}
static int lua_LiveEntity_GetMotionType( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushnumber( L, pLiveEntity->GetMotionType() );
	return 1;

}


static int lua_LiveEntity_IsIdle( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsIdle() );
	return 1;

}
static int lua_LiveEntity_IsMoving( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsMoving() );
	return 1;

}
static int lua_LiveEntity_IsLeaning( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsLeaning() );
	return 1;

}
static int lua_LiveEntity_IsWalking( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsWalking() );
	return 1;

}
static int lua_LiveEntity_IsRunning( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsRunning() );
	return 1;

}
static int lua_LiveEntity_IsJumping( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsJumping() );
	return 1;

}
static int lua_LiveEntity_IsHanging( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsHanging() );
	return 1;

}
static int lua_LiveEntity_IsFalling( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsFalling() );
	return 1;

}
static int lua_LiveEntity_IsCrouched( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsCrouched() );
	return 1;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int lua_LiveEntity_IsPlainWalking( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsPlainWalking() );
	return 1;

}
static int lua_LiveEntity_IsFlying( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsFlying() );
	return 1;

}
static int lua_LiveEntity_IsClimbing( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsClimbing() );
	return 1;

}
static int lua_LiveEntity_IsSwimming( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushboolean( L, pLiveEntity->IsSwimming() );
	return 1;

}


static int lua_LiveEntity_SetMoving( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TBOOLEAN );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetMoving( lua_toboolean( L, 2 ) ? true : false );
	return 0;

}
static int lua_LiveEntity_SetLeaning( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TBOOLEAN );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetLeaning( lua_toboolean( L, 2 ) ? true : false );
	return 0;

}
static int lua_LiveEntity_SetWalking( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TBOOLEAN );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetWalking( lua_toboolean( L, 2 ) ? true : false );
	return 0;

}
static int lua_LiveEntity_SetRunning( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TBOOLEAN );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetRunning( lua_toboolean( L, 2 ) ? true : false );
	return 0;

}
static int lua_LiveEntity_SetJumping( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TBOOLEAN );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetJumping( lua_toboolean( L, 2 ) ? true : false );
	return 0;

}
static int lua_LiveEntity_SetHanging( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TBOOLEAN );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetHanging( lua_toboolean( L, 2 ) ? true : false );
	return 0;

}
static int lua_LiveEntity_SetFalling( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TBOOLEAN );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetFalling( lua_toboolean( L, 2 ) ? true : false );
	return 0;

}
static int lua_LiveEntity_SetCrouched( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TBOOLEAN );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetCrouched( lua_toboolean( L, 2 ) ? true : false );
	return 0;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static int lua_LiveEntity_SetPlainWalking( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetPlainWalking();
	return 0;

}
static int lua_LiveEntity_SetFlying( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetFlying();
	return 0;

}
static int lua_LiveEntity_SetSwimming( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetSwimming();
	return 0;

}

static int lua_LiveEntity_SetClimbing( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetClimbing();
	return 0;

}


static int lua_LiveEntity_ClearStates( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->ClearState();
	return 0;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int lua_LiveEntity_SetWalkSpeed( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetWalkSpeed( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_LiveEntity_GetWalkSpeed( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushnumber( L, pLiveEntity->GetWalkSpeed() );
	return 1;

}

static int lua_LiveEntity_SetRunSpeed( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetRunSpeed( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_LiveEntity_GetRunSpeed( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushnumber( L, pLiveEntity->GetRunSpeed() );
	return 1;

}

static int lua_LiveEntity_SetCrouchSpeed( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetCrouchSpeed( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_LiveEntity_GetCrouchSpeed( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushnumber( L, pLiveEntity->GetCrouchSpeed() );
	return 1;

}

static int lua_LiveEntity_SetJumpForce( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetJumpForce( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_LiveEntity_GetJumpForce( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushnumber( L, pLiveEntity->GetJumpForce() );
	return 1;

}

static int lua_LiveEntity_SetMaxItemMass( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	pLiveEntity->SetMaxItemMass( lua_tonumber( L, 2 ) );
	return 0;

}
static int lua_LiveEntity_GetMaxItemMass( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	lua_pushnumber( L, pLiveEntity->GetMaxItemMass() );
	return 1;

}

static int lua_LiveEntity_GetInventory( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	Engine()->ScriptEngine()->PushUserdata( pLiveEntity->GetInventory(), "cInventory" );
	return 1;

}

static int lua_LiveEntity_GetFlashLight( lua_State *L ) {

	cLiveEntity *pLiveEntity = Engine()->ScriptEngine()->GetUserdata< cLiveEntity >( 1 );
	Engine()->ScriptEngine()->PushUserdata( pLiveEntity->GetFlashLight(), "cFlashLight" );
	return 1;

}


bool LUA_Expose_LiveEntity( lua_State *L ) {

	LUAMetaTable LiveEntity = LUAMetaTable( L, "cLiveEntity" );
	//				  Module Function name			Returned		Arguments
	LUA_CLASS_METHOD( LiveEntity, IsHuman,			"boolean",		"void" );

	LUA_CLASS_METHOD( LiveEntity, SetHealth,		"void",			"float" );
	LUA_CLASS_METHOD( LiveEntity, GetHealth,		"float",		"void" );
	LUA_CLASS_METHOD( LiveEntity, SetStamina,		"void",			"float" );
	LUA_CLASS_METHOD( LiveEntity, GetStamina,		"float",		"void" );
	LUA_CLASS_METHOD( LiveEntity, SetViewRange,		"void",			"float" );
	LUA_CLASS_METHOD( LiveEntity, GetViewRange,		"float",		"void" );

	// Get States
	LUA_CLASS_METHOD( LiveEntity, IsIdle,			"boolean",		"void" );
	LUA_CLASS_METHOD( LiveEntity, IsMoving,			"boolean",		"void" );
	LUA_CLASS_METHOD( LiveEntity, IsLeaning,		"boolean",		"void" );
	LUA_CLASS_METHOD( LiveEntity, IsWalking,		"boolean",		"void" );
	LUA_CLASS_METHOD( LiveEntity, IsRunning,		"boolean",		"void" );
	LUA_CLASS_METHOD( LiveEntity, IsJumping,		"boolean",		"void" );
	LUA_CLASS_METHOD( LiveEntity, IsHanging,		"boolean",		"void" );
	LUA_CLASS_METHOD( LiveEntity, IsFalling,		"boolean", 		"void" );
	LUA_CLASS_METHOD( LiveEntity, IsCrouched,		"boolean", 		"void" );

	LiveEntity.Constant( "MOTION_FLY",		LIVE_ENTITY::Motion::Fly );
	LiveEntity.Constant( "MOTION_SWIM",		LIVE_ENTITY::Motion::Swim );
	LiveEntity.Constant( "MOTION_WALK",		LIVE_ENTITY::Motion::Walk );
	LiveEntity.Constant( "MOTION_P1TOP2",	LIVE_ENTITY::Motion::P1ToP2 );

	// Get Motion types
	LUA_CLASS_METHOD( LiveEntity, IsPlainWalking,	"boolean", 		"void" );
	LUA_CLASS_METHOD( LiveEntity, IsFlying,			"boolean", 		"void" );
	LUA_CLASS_METHOD( LiveEntity, IsSwimming,		"boolean", 		"void" );
	LUA_CLASS_METHOD( LiveEntity, IsClimbing,		"boolean", 		"void" );

	// Set States
	LUA_CLASS_METHOD( LiveEntity, SetMoving,		"void", 		"boolean" );
	LUA_CLASS_METHOD( LiveEntity, SetLeaning,		"void", 		"boolean" );
	LUA_CLASS_METHOD( LiveEntity, SetWalking,		"void", 		"boolean" );
	LUA_CLASS_METHOD( LiveEntity, SetRunning,		"void", 		"boolean" );
	LUA_CLASS_METHOD( LiveEntity, SetJumping,		"void", 		"boolean" );
	LUA_CLASS_METHOD( LiveEntity, SetHanging,		"void", 		"boolean" );
	LUA_CLASS_METHOD( LiveEntity, SetFalling,		"void", 		"boolean" );
	LUA_CLASS_METHOD( LiveEntity, SetCrouched,		"void", 		"boolean" );

	// Set Motion types
	LUA_CLASS_METHOD( LiveEntity, SetPlainWalking,	"void", 		"void" );
	LUA_CLASS_METHOD( LiveEntity, SetFlying,		"void", 		"void" );
	LUA_CLASS_METHOD( LiveEntity, SetSwimming,		"void", 		"void" );
	LUA_CLASS_METHOD( LiveEntity, SetClimbing,		"void", 		"void" );

	LUA_CLASS_METHOD( LiveEntity, ClearStates,		"void", 		"void" );

	LUA_CLASS_METHOD( LiveEntity, SetWalkSpeed,		"void",			"float" );
	LUA_CLASS_METHOD( LiveEntity, GetWalkSpeed,		"float",		"void" );
	LUA_CLASS_METHOD( LiveEntity, SetRunSpeed,		"void",			"float" );
	LUA_CLASS_METHOD( LiveEntity, GetRunSpeed,		"float",		"void" );
	LUA_CLASS_METHOD( LiveEntity, SetCrouchSpeed,	"void",			"float" );
	LUA_CLASS_METHOD( LiveEntity, GetCrouchSpeed,	"float",		"void" );
	LUA_CLASS_METHOD( LiveEntity, SetJumpForce,		"void",			"float" );
	LUA_CLASS_METHOD( LiveEntity, GetJumpForce,		"float",		"void" );

	LUA_CLASS_METHOD( LiveEntity, SetMaxItemMass,	"void",			"float" );
	LUA_CLASS_METHOD( LiveEntity, GetMaxItemMass,	"float",		"void" );

	LUA_CLASS_METHOD( LiveEntity, GetInventory,		"cInventory",	"void" );
	LUA_CLASS_METHOD( LiveEntity, GetFlashLight,	"cFlashLight",	"void" );

	if ( !LiveEntity.Close() ) {
		cMsgCRT( "LUA_Expose_LiveEntity: cLiveEntity exposure fail" );
		return false;
	}

//	LUANameSpace 

	return true;
}