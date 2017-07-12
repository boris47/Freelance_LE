

#pragma once


#include <Engine.h>		// Utils\LTXReader

#include "ScriptEngine\ScriptEngine.h"
#include "ScriptEngine\ScriptExtension.h"


bool LUA_Expose_LTXReader( lua_State *L );


static int lua_List_GetData( lua_State *L ) {

	cList *pList = Engine()->ScriptEngine()->GetUserdata< cList >( 1 );
	std::vector < cLineValue* > vData = pList->GetData();
	lua_newtable( L );									// ..., Table
	int Index = 1;
	for ( cLineValue *pLineValue : vData ) {
		lua_pushstring( L, pLineValue->Key().c_str() );	// ..., Table, String
		lua_rawseti( L, -2, Index );					// ..., Table
		Index++;
	}
	return 1;

}




//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////




static int lua_Section_Lines( lua_State *L ) {

	cSection *pSection = Engine()->ScriptEngine()->GetUserdata< cSection >( 1 );
	lua_pushnumber( L, pSection->Lines() );
	return 1;

}

static int lua_Section_HasKey( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cSection *pSection = Engine()->ScriptEngine()->GetUserdata< cSection >( 1 );
	lua_pushboolean( L, pSection->HasKey( lua_tostring( L, 2 ) ) );
	return 1;

}

static int lua_Section_GetRawValue( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cSection *pSection = Engine()->ScriptEngine()->GetUserdata< cSection >( 1 );
	lua_pushstring( L, pSection->GetRawValue( lua_tostring( L, 2 ) ).c_str() );
	return 1;

}

static int lua_Section_bGetInt( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cSection *pSection = Engine()->ScriptEngine()->GetUserdata< cSection >( 1 );

	int iValue;
	int iDefault = lua_tonumber( L, 3 ) || -1;

	bool bResult = pSection->bGetInt( lua_tostring( L, 2 ), iValue, iDefault );
	lua_pushboolean( L, bResult ? 1 : 0 );
	lua_pushnumber( L, bResult ? iValue : iDefault );
	return 2;

}

static int lua_Section_bGetBool( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cSection *pSection = Engine()->ScriptEngine()->GetUserdata< cSection >( 1 );

	bool bValue;
	bool bDefault = lua_toboolean( L, 3 ) ? true : false;

	bool bResult = pSection->bGetBool( lua_tostring( L, 2 ), bValue, bDefault );
	lua_pushboolean( L, bResult ? 1 : 0 );
	lua_pushboolean( L, bResult ? bValue : bDefault );
	return 2;

}

static int lua_Section_bGetFloat( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cSection *pSection = Engine()->ScriptEngine()->GetUserdata< cSection >( 1 );

	float fValue;
	float fDefault = lua_tonumber( L, 3 ) || -1.0;

	bool bResult = pSection->bGetFloat( lua_tostring( L, 2 ), fValue, fDefault );
	lua_pushboolean( L, bResult ? 1 : 0 );
	lua_pushnumber( L, bResult ? fValue : fDefault );
	return 2;

}

static int lua_Section_bGetString( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cSection *pSection = Engine()->ScriptEngine()->GetUserdata< cSection >( 1 );

	std::string sValue;
	std::string sDefault = "";
	if ( lua_isstring( L, 3 ) ) sDefault = lua_tostring( L, 3 );

	bool bResult = pSection->bGetString( lua_tostring( L, 2 ), sValue, sDefault );
	lua_pushboolean( L, bResult ? 1 : 0 );
	lua_pushstring( L, std::string( bResult ? sValue : sDefault ).c_str() );
	return 2;

}


static int lua_Section_bGetVec2( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cSection *pSection = Engine()->ScriptEngine()->GetUserdata< cSection >( 1 );

	Leadwerks::Vec2 vValue( 0.0f );

	bool bResult = pSection->bGetVec2( lua_tostring( L, 2 ), vValue );
	lua_pushboolean( L, bResult ? 1 : 0 );
	Leadwerks::Interpreter::PushVec2( vValue );
	return 2;

}

static int lua_Section_bGetVec3( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cSection *pSection = Engine()->ScriptEngine()->GetUserdata< cSection >( 1 );

	Leadwerks::Vec3 vValue( 0.0f );

	bool bResult = pSection->bGetVec3( lua_tostring( L, 2 ), vValue );
	lua_pushboolean( L, bResult ? 1 : 0 );
	Leadwerks::Interpreter::PushVec3( vValue );
	return 2;

}

static int lua_Section_bGetVec4( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cSection *pSection = Engine()->ScriptEngine()->GetUserdata< cSection >( 1 );

	Leadwerks::Vec4 vValue( 0.0f );

	bool bResult = pSection->bGetVec4( lua_tostring( L, 2 ), vValue );
	lua_pushboolean( L, bResult ? 1 : 0 );
	Leadwerks::Interpreter::PushVec4( vValue );
	return 2;

}




//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////



/* SE RICEVE UNA STRINGA:
SE READER GIA' INSTANZIATO
PROVA A CARICARE IL FILE
SE FALLISCE RITORNA NIL
ALTRIMENTI
RITORNA L'INSTANZIA GIA' CREATA
|
ALTRIMENTI
CREA NUOVA INSTANZA E PROVA A CARICARE IL FILE
SE FALLISCE RITORNA NIL
ALTRIMENTI
RITORNA L'INSTANZIA CREATA
|
|
ALTRIMENTI
SE READER GIA' INSTANZIATO
RITORNA L'INSTANZIA CREATA
ALTRIMENTI
CREA NUOVA INSTANZA
|
|
*/
static int lua_Reader_new( lua_State *L ) {

	cLTXReader *pLTX_Reader;
	if ( lua_isstring( L, 2 ) ) {
		if ( pLTXReader ) {
			if ( !pLTXReader->LoadFile( lua_tostring( L, 2 ) ) ) {
				return luaL_error( L, "[ LUA ] LTXReader cannot load file %s", lua_tostring( L, 2 ) );
			}
			pLTX_Reader = pLTXReader;
		} else {
			pLTX_Reader = new cLTXReader();
			pLTX_Reader->SetLUA_Allocated( true );
			if ( !pLTX_Reader->LoadFile( lua_tostring( L, 2 ) ) ) {
				return luaL_error( L, "[ LUA ] New LTXReader cannot load file %s", lua_tostring( L, 2 ) );
			}
		}
	} else {
		if ( pLTXReader ) pLTX_Reader = pLTXReader;
		else {
			pLTX_Reader = new cLTXReader();
			pLTX_Reader->SetLUA_Allocated( true );
		}
	}

	Engine()->ScriptEngine()->PushUserdata( pLTX_Reader, "cLTXReader" );
	return 1;

}

static int lua_Reader_fin( lua_State *L ) {

	cLTXReader *pLTX_Reader = Engine()->ScriptEngine()->GetUserdata< cLTXReader >( 1 );
	if ( pLTX_Reader->IsLUA_Allocated() ) SAFE_DELETE( pLTX_Reader );

	lua_gc( L, LUA_GCCOLLECT, 0 );	// performs a full garbage-collection cycle
	return 0;
}

static int lua_Reader_LoadFile( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cLTXReader *pLTX_Reader = Engine()->ScriptEngine()->GetUserdata< cLTXReader >( 1 );
	lua_pushboolean( L, ( pLTX_Reader->LoadFile( lua_tostring( L, 2 ) ) ) ? 1 : 0 );
	return 1;

}

static int lua_Reader_Loaded( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cLTXReader *pLTX_Reader = Engine()->ScriptEngine()->GetUserdata< cLTXReader >( 1 );
	lua_pushboolean( L, ( pLTX_Reader->Loaded( lua_tostring( L, 2 ) ) ) ? 1 : 0 );
	return 1;

}


static int lua_Reader_GetSection( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cLTXReader *pLTX_Reader = Engine()->ScriptEngine()->GetUserdata< cLTXReader >( 1 );
	cSection *pSection = pLTX_Reader->GetSection( lua_tostring( L, 2 ) );
	if ( pSection ) {
		Engine()->ScriptEngine()->PushUserdata( pSection, "cSection" );
		return 1;
	}
	lua_pushnil( L );
	return 1;

}

static int lua_Reader_GetList( lua_State *L ) {

	luaL_checktype( L, 2, LUA_TSTRING );
	cLTXReader *pLTX_Reader = Engine()->ScriptEngine()->GetUserdata< cLTXReader >( 1 );
	cList *pList = pLTX_Reader->GetList( std::string( lua_tostring( L, 2 ) ) );
	if ( pList ) {
		Engine()->ScriptEngine()->PushUserdata( pList, "cList" );
		return 1;
	}
	lua_pushnil( L );
	return 1;

}

static int lua_Reader_PrintMap( lua_State *L ) {

	cLTXReader *pLTX_Reader = Engine()->ScriptEngine()->GetUserdata< cLTXReader >( 1 );
	if ( lua_isstring( L, 2 ) )
		pLTX_Reader->PrintMap( lua_tostring( L, 2 ) );
	else
		pLTX_Reader->PrintMap();

	return 0;
}




bool LUA_Expose_LTXReader( lua_State *L ) {

	LUAMetaTable List = LUAMetaTable( L, "cList" );
	//				  Module Function name			Returned		Arguments
	LUA_CLASS_METHOD( List,  GetData,				"table",		"void" );

	if ( !List.Close() ) {
		cMsgCRT( "LUA_Expose_LTXReader: cList exposure fail" );
		return false;
	}

	LUAMetaTable Section = LUAMetaTable( L, "cSection" );
	//				  Module Function name			Returned				Arguments
	LUA_CLASS_METHOD( Section, Lines,				"int",					"void" );
	LUA_CLASS_METHOD( Section, HasKey,				"bool",					"string" );
	LUA_CLASS_METHOD( Section, GetRawValue,			"string",				"string" );
	LUA_CLASS_METHOD( Section, bGetInt,				"boolean, int",			"string, (int)" );
	LUA_CLASS_METHOD( Section, bGetBool,			"boolean, boolean",		"string, (boolean)" );
	LUA_CLASS_METHOD( Section, bGetFloat,			"boolean, float",		"string, (float)" );
	LUA_CLASS_METHOD( Section, bGetString,			"boolean, string",		"string, (string)" );
	LUA_CLASS_METHOD( Section, bGetVec2,			"bool, Vec2",			"string" );
	LUA_CLASS_METHOD( Section, bGetVec3,			"bool, Vec3",			"string" );
	LUA_CLASS_METHOD( Section, bGetVec4,			"bool, Vec4",			"string" );

	if ( !Section.Close() ) {
		cMsgCRT( "LUA_Expose_LTXReader: cSection exposure fail" );
		return false;
	}

	LUAClass Reader = LUAClass( L, "LTXReader", lua_Reader_new, lua_Reader_fin );
	//				  Module  Function name			Returned	Arguments
	LUA_CLASS_METHOD( Reader, LoadFile,				"boolean",	"string" );
	LUA_CLASS_METHOD( Reader, Loaded,				"boolean",	"string" );
	LUA_CLASS_METHOD( Reader, GetSection,			"cSection", "string" );
	LUA_CLASS_METHOD( Reader, GetList,				"cList",	"string" );
	LUA_CLASS_METHOD( Reader, PrintMap,				"void",		"(string)" );

	if ( !Reader.Close() ) {
		cMsgCRT( "LUA_Expose_LTXReader: cLTXReader exposure fail" );
		return false;
	}

	return true;
}

