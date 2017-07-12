

#pragma once


#include <Engine.h>		// Audio\cAudio

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_AudioDevice( lua_State *L );




static int lua_AudioSrc_IsPlaying( lua_State *L ) {

	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 1 );
	lua_pushboolean( L, pAudioSrc->IsPlaying() );
	return 1;

}

static int lua_AudioSrc_SetVolume( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 1 );
	pAudioSrc->SetVolume( lua_tonumber( L, 2 ) );
	lua_settop( L, 1 );
	return 1;

}

static int lua_AudioSrc_GetVolume( lua_State *L ) {

	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 1 );
	lua_pushnumber( L, pAudioSrc->GetVolume() );
	return 1;

}

static int lua_AudioSrc_SetPitch( lua_State *L ) {
	
	luaL_checktype( L, 2, LUA_TNUMBER );
	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 1 );
	pAudioSrc->SetPitch( lua_tonumber( L, 2 ) );
	lua_settop( L, 1 );
	return 1;

}

static int lua_AudioSrc_GetPitch( lua_State *L ) {

	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 1 );
	lua_pushnumber( L, pAudioSrc->GetPitch() );
	return 1;

}

static int lua_AudioSrc_FadeIn( lua_State *L ) {

	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 1 );
	pAudioSrc->FadeIn( lua_tonumber( L, 2 ), lua_toboolean( L, 3 ) ? true : false );
	lua_settop( L, 1 );
	return 1;

}

static int lua_AudioSrc_FadeOut( lua_State *L ) {

	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 1 );
	pAudioSrc->FadeOut( lua_tonumber( L, 2 ) );
	return 0;

}

static int lua_AudioSrc_Play2D( lua_State *L ) {

	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 1 );
	pAudioSrc->Play2D( lua_toboolean( L, 2 ) ? true : false );
	
	lua_settop( L, 1 );
	return 1;

}

static int lua_AudioSrc_Play3D( lua_State *L ) {

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 2, "Vec3", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'SetRotation'.", &tolua_err );
		return 0;
	}

	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 1 );
	Leadwerks::Vec3 vPosition = *( Leadwerks::Vec3* ) tolua_tousertype( L, 2, 0 );
	pAudioSrc->Play3D( vPosition, lua_toboolean( L, 3 ) ? true : false );
	
	lua_settop( L, 1 );
	return 1;

}




//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////




static int lua_AudioDevice_SetMainVolume( lua_State *L ) {
	luaL_checktype( L, 2, LUA_TNUMBER );
	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );
	if ( pAudioDevice ) pAudioDevice->SetMainVolume( lua_tonumber( L, 2 ) );
	return 1;
}

static int lua_AudioDevice_GetMainVolume( lua_State *L ) {

	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );
	if ( pAudioDevice ) lua_pushnumber( L, pAudioDevice->GetMainVolume() );
	return 1;

}

static int lua_AudioDevice_SetMusicVolume( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );
	if ( pAudioDevice ) pAudioDevice->SetMusicVolume( lua_tonumber( L, 2 ) );
	return 1;

}

static int lua_AudioDevice_GetMusicVolume( lua_State *L ) {

	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );
	if ( pAudioDevice ) lua_pushnumber( L, pAudioDevice->GetMusicVolume() );
	return 1;

}

static int lua_AudioDevice_SetSoundsVolume( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TNUMBER );
	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );
	if ( pAudioDevice ) pAudioDevice->SetSoundsVolume( lua_tonumber( L, 2 ) );
	return 1;

}

static int lua_AudioDevice_GetSoundsVolume( lua_State *L ) {

	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );
	if ( pAudioDevice ) lua_pushnumber( L, pAudioDevice->GetSoundsVolume() );
	return 1;

}




static int lua_AudioDevice_GetListener( lua_State *L ) {

	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );
	tolua_pushusertype( L, pAudioDevice->GetListener(), "Listener" );
	return 1;

}


static int lua_AudioDevice_FindSource( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );
	Engine()->ScriptEngine()->PushUserdata( pAudioDevice->FindSource( lua_tostring( L, 2 ) ), "cAudioSrc" );
	return 1;

}


static int lua_AudioDevice_LoadSource( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );
	Engine()->ScriptEngine()->PushUserdata( pAudioDevice->LoadSource( lua_tostring( L, 2 ) ), "cAudioSrc" );
	return 1;

}


static int lua_AudioDevice_Stop( lua_State *L ) {

	luaL_checkany( L, 2 ); // string or userdata
	if ( !lua_isstring( L, 2 ) && !lua_isuserdata( L, 2 ) )
		return luaL_typerror( L, 2, "String or AudioSrc" );

	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );

	if ( lua_isstring( L, 2 ) ) {	// Stop( "test" )
		pAudioDevice->Stop( lua_tostring( L, 2 ) );
		return 0;
	}
	
	// if not string must be an userdata
	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 2 );
	pAudioDevice->Stop( pAudioSrc );
	return 0;

}

static int lua_AudioDevice_Play2D( lua_State *L ) {

	luaL_checkany( L, 2 ); // string or userdata
	if ( !lua_isstring( L, 2 ) && !lua_isuserdata( L, 2 ) )
		return luaL_typerror( L, 2, "String or AudioSrc" );

	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );

	bool bLoop = lua_toboolean( L, 3 ) ? true : false;

	if ( lua_isstring( L, 2 ) ) {	// Play2D( "test" )
		cAudioSrc *pAudioSrc = pAudioDevice->Play2D( lua_tostring( L, 2 ), bLoop );
		Engine()->ScriptEngine()->PushUserdata( pAudioSrc, "cAudioSrc" );
		return 1;
	}

	// if not string must be an userdata
	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 2 );
	pAudioDevice->Play2D( pAudioSrc, bLoop );
	// Class, Userdata, Boolean
	lua_settop( L, 2 ); // Leave userdata in the stack for re-use
	// Class, Userdata
	return 1;	// return userdata itself


}


static int lua_AudioDevice_Play3D( lua_State *L ) {

	luaL_checkany( L, 2 ); // string or userdata
	if ( !lua_isstring( L, 2 ) && !lua_isuserdata( L, 2 ) )
		return luaL_typerror( L, 2, "String or AudioSrc" );

	tolua_Error tolua_err;
	if ( !tolua_isusertype( L, 3, "Vec3", 0, &tolua_err ) ) {
		tolua_error( L, "#ferror in function 'Play3D'.", &tolua_err );
		return 0;
	}

	Leadwerks::Vec3 vPosition = *( Leadwerks::Vec3 * ) tolua_tousertype( L, 3, 0 );

	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );

	bool bLoop = lua_toboolean( L, 4 ) ? true : false;

	if ( lua_isstring( L, 2 ) ) {	// Play3D( "test", { 15, 12, 16 } )
		cAudioSrc *pAudioSrc = pAudioDevice->Play3D( lua_tostring( L, 2 ), vPosition, bLoop );
		Engine()->ScriptEngine()->PushUserdata( pAudioSrc, "cAudioSrc" );
		return 1;
	}

	// if not string must be an userdata
	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 2 );
	pAudioDevice->Play3D( pAudioSrc, vPosition, bLoop );
	// Class, Userdata, Vec3
	lua_settop( L, 2 ); // Leave userdata in the stack for re-use
	// Class, Userdata
	return 1;	// return userdata itself

}

static int lua_AudioDevice_PlayMusic( lua_State *L ) {

	luaL_checkany( L, 2 ); // string or userdata
	if ( !lua_isstring( L, 2 ) && !lua_isuserdata( L, 2 ) )
		return luaL_typerror( L, 2, "String or AudioSrc" );

	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );

	bool bLoop = lua_toboolean( L, 3 ) ? true : false;

	if ( lua_isstring( L, 2 ) ) {	// PlayMusic( "test" )
		cAudioSrc *pAudioSrc = pAudioDevice->PlayMusic( lua_tostring( L, 2 ), bLoop );
		Engine()->ScriptEngine()->PushUserdata( pAudioSrc, "cAudioSrc" );
		return 1;
	}

	// if not string must be an userdata
	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 2 );
	pAudioDevice->PlayMusic( pAudioSrc, bLoop );
	// Class, Userdata, Boolean
	lua_settop( L, 2 ); // Leave userdata in the stack for re-use
	// Class, Userdata
	return 1;	// return userdata itself

}

static int lua_AudioDevice_FadeIn( lua_State *L ) {

	luaL_checkany( L, 2 ); // string or userdata

	if ( !lua_isstring( L, 2 ) && !lua_isuserdata( L, 2 ) )
		return luaL_typerror( L, 2, "String or AudioSrc" );

	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );

	float fMilliseconds = lua_tonumber( L, 3 ) || 2000;
	bool bLoop = lua_toboolean( L, 4 ) ? true : false;

	if ( lua_isstring( L, 2 ) ) {	// FadeIn( "test", 3000, true )
		cAudioSrc *pAudioSrc = pAudioDevice->FadeIn( lua_tostring( L, 2 ), fMilliseconds, bLoop );
		Engine()->ScriptEngine()->PushUserdata( pAudioSrc, "cAudioSrc" );
		return 1;
	}

	// if not string must be an userdata
	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 2 );
	pAudioDevice->FadeIn( pAudioSrc, fMilliseconds, bLoop );
	// Class, Userdata, Number, Boolean
	lua_settop( L, 2 ); // Leave userdata in the stack for re-use
	// Class, Userdata
	return 1;	// return userdata itself

}

static int lua_AudioDevice_FadeOut( lua_State *L ) {

	luaL_checkany( L, 2 ); // string or userdata

	if ( !lua_isstring( L, 2 ) && !lua_isuserdata( L, 2 ) )
		return luaL_typerror( L, 2, "String or AudioSrc" );

	cAudioDevice *pAudioDevice = Engine()->ScriptEngine()->GetUserdata< cAudioDevice >( 1 );

	float fMilliseconds = lua_tonumber( L, 3 ) || 2000;

	if ( lua_isstring( L, 2 ) ) {	// FadeIn( "test", { 15, 12, 16 } )
		pAudioDevice->FadeOut( lua_tostring( L, 2 ), fMilliseconds );
		return 0;
	}

	// if not string must be an userdata
	cAudioSrc *pAudioSrc = Engine()->ScriptEngine()->GetUserdata< cAudioSrc >( 2 );
	pAudioDevice->FadeOut( pAudioSrc, fMilliseconds );
	return 0;

}



bool LUA_Expose_AudioDevice( lua_State *L ) {
	
	LUAMetaTable AudioSrc = LUAMetaTable( L, "cAudioSrc" );
	//					Module	Function name	Returned								Arguments
	LUA_CLASS_METHOD( AudioSrc, IsPlaying,		"boolean",								"void" );
	LUA_CLASS_METHOD( AudioSrc, SetVolume,		"cAudioSrc",							"float" );
	LUA_CLASS_METHOD( AudioSrc, GetVolume,		"float",								"void" );
	LUA_CLASS_METHOD( AudioSrc, SetPitch,		"cAudioSrc",							"float" );
	LUA_CLASS_METHOD( AudioSrc, GetPitch,		"float",								"void" );
	LUA_CLASS_METHOD( AudioSrc, FadeIn,			"cAudioSrc",							"int, bool" );
	LUA_CLASS_METHOD( AudioSrc, FadeOut,		"void",									"void" );
	LUA_CLASS_METHOD( AudioSrc, Play2D,			"cAudioSrc",							"bool" );
	LUA_CLASS_METHOD( AudioSrc, Play3D,			"cAudioSrc",							"Vec3, bool" );

	if ( !AudioSrc.Close() ) {
		cMsgCRT( "LUA_Expose_AudioDevice: cAudioSrc exposure fail" );
		return false;
	}
	

	// Returned by engine
	LUAClass AudioDevice = LUAClass( L, "AudioDevice" );
	//					Module		Function name	Returned								Arguments
	LUA_CLASS_METHOD( AudioDevice, SetMainVolume,	"void",									"float" );
	LUA_CLASS_METHOD( AudioDevice, GetMainVolume,	"float",								"void" );
	LUA_CLASS_METHOD( AudioDevice, SetMusicVolume,	"void",									"float" );
	LUA_CLASS_METHOD( AudioDevice, GetMusicVolume,	"float",								"void" );
	LUA_CLASS_METHOD( AudioDevice, SetSoundsVolume, "void",									"float" );
	LUA_CLASS_METHOD( AudioDevice, GetSoundsVolume, "float",								"void" );
	LUA_CLASS_METHOD( AudioDevice, GetListener,		"Listener",								"void" );
	LUA_CLASS_METHOD( AudioDevice, FindSource,		"cAudioSrc",							"string" );
	LUA_CLASS_METHOD( AudioDevice, LoadSource,		"cAudioSrc",							"string" );
	LUA_CLASS_METHOD( AudioDevice, Stop,			"void",									"cAudioSrc or string" );
	LUA_CLASS_METHOD( AudioDevice, Play2D,			"cAudioSrc",							"cAudioSrc or string, boolean" );
	LUA_CLASS_METHOD( AudioDevice, Play3D,			"cAudioSrc",							"cAudioSrc or string, Vec3, boolean" );
	LUA_CLASS_METHOD( AudioDevice, PlayMusic,		"cAudioSrc",							"cAudioSrc or string, boolean" );
	LUA_CLASS_METHOD( AudioDevice, FadeIn,			"cAudioSrc",							"cAudioSrc or string, int, boolean" );
	LUA_CLASS_METHOD( AudioDevice, FadeOut,			"void",									"cAudioSrc or string, int" );
	
	if ( !AudioDevice.Close() ) {
		cMsgCRT( "LUA_Expose_AudioDevice: cAudio exposure fail" );
		return false;
	}
	
	return true;
	
}