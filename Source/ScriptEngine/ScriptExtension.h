
#pragma once

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include <string>


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

bool Lua_Constant( lua_State *L, std::string Name, double Val );

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


bool LUA_Class_Open( lua_State *L, std::string Name, lua_CFunction CallFunction, lua_CFunction GCFunction );
bool LUA_Class_Register( lua_State *L, std::string Namespace, std::string FuncName, lua_CFunction Function, std::string sReturned, std::string sDescription );
void LUA_Class_Close( lua_State *L );

struct LUAClass {
	private:
		lua_State *pState;
		std::string sClassName;
		bool bIsOK = false;
	
	public:
		inline LUAClass( lua_State *L, std::string Name, lua_CFunction pInit = NULL, lua_CFunction pFinal = NULL );
		inline LUAClass	&	Method( std::string Name, lua_CFunction pFunction, std::string sReturned, std::string sDescription );
		inline LUAClass &	Constant( std::string Name, double Val );
		inline bool			Close();
};


LUAClass::LUAClass ( lua_State *L, std::string ClassName, lua_CFunction pInit, lua_CFunction pFinal ) {

	pState = L;
	sClassName = ClassName;
	bIsOK = LUA_Class_Open( pState, ClassName, pInit, pFinal );

}

LUAClass &LUAClass::Method( std::string Name, lua_CFunction pFunction, std::string sReturned, std::string sDescription ) {

	// Only if class creation or last registration has good end
	if ( bIsOK ) bIsOK = LUA_Class_Register( pState, sClassName, Name, pFunction, sReturned, sDescription );
	return ( *this );

}

LUAClass &LUAClass::Constant( std::string Name, double Val ) {

	if ( bIsOK ) bIsOK = Lua_Constant( pState, Name, Val );
	return ( *this );

}

bool LUAClass::Close() {
	LUA_Class_Close( pState );
	return bIsOK;
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


bool LUA_MetaTable_Open( lua_State *L, std::string Type );
bool LUA_MetaTable_Register( lua_State *L, std::string FuncName, lua_CFunction pFunction, std::string sReturned, std::string sDescription );
void LUA_MetaTable_Close( lua_State *L );

struct LUAMetaTable {
private:
	lua_State *pState;
	bool bIsOK = false;

public:
	inline LUAMetaTable( lua_State *L, std::string Name );
	inline LUAMetaTable &Method( std::string Name, lua_CFunction pFunction, std::string sReturned, std::string sDescription );
	inline LUAMetaTable &Constant( std::string Name, double Val );
	inline bool Close();
};

LUAMetaTable::LUAMetaTable( lua_State *L, std::string Name ) {

	pState = L;
	bIsOK = LUA_MetaTable_Open( pState, Name );

}

LUAMetaTable &LUAMetaTable::Method( std::string Name, lua_CFunction pFunction, std::string sReturned, std::string sDescription ) {

	if ( bIsOK ) bIsOK = LUA_MetaTable_Register( pState, Name, pFunction, sReturned, sDescription );
	return ( *this );

}

LUAMetaTable &LUAMetaTable::Constant( std::string Name, double Val ) {

	if ( bIsOK ) bIsOK = Lua_Constant( pState, Name, Val );
	return ( *this );

}

bool LUAMetaTable::Close() {
	LUA_MetaTable_Close( pState );
	return bIsOK;
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

bool LUA_NameSpace_Open( lua_State *L, std::string Name );

struct LUANameSpace {
private:
	lua_State *pState;
	bool bIsOK = false;

public:
	inline LUANameSpace( lua_State *L, std::string Name );
	inline LUANameSpace &Method( std::string Name, lua_CFunction pFunction, std::string sReturned, std::string sDescription );
	inline LUANameSpace &Constant( std::string Name, double Val );
	inline bool Close();
};

LUANameSpace::LUANameSpace( lua_State *L, std::string Name ) {

	pState = L;
	bIsOK = LUA_NameSpace_Open( pState, Name );

}

LUANameSpace &LUANameSpace::Method( std::string Name, lua_CFunction pFunction, std::string sReturned, std::string sDescription ) {

	if ( bIsOK ) bIsOK = LUA_MetaTable_Register( pState, Name, pFunction, sReturned, sDescription );
	return ( *this );

}

LUANameSpace &LUANameSpace::Constant( std::string Name, double Val ) {

	if ( bIsOK ) bIsOK = Lua_Constant( pState, Name, Val );
	return ( *this );

}

bool LUANameSpace::Close() {
	LUA_MetaTable_Close( pState );
	return bIsOK;
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


void LUA_RegisterExtensions( lua_State *L );