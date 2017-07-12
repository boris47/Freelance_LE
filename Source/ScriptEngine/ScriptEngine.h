
#pragma once

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include <list>
#include "..\Defines.h"	// LUA_SCRIPTS_PATH

void LUA_PrintStack( lua_State *L );

#define LUA_CLASS_METHOD(c,f,r,d)		c.Method( #f, lua ## _ ## c ## _ ## f, r, d )
#define LUA_CLASS_CONSTANT(c,n,v)		c.Constant( #n, v )

class cScriptEngine {
	private:
		lua_State *					L = NULL;
		std::list < std::string >	lLoaded;
		
		bool						bIsOK = false;
	
	public:
		cScriptEngine();
		inline	bool		IsOK()		{ return bIsOK; }
		inline	lua_State *	GetState()	{ return L; }

	private:
				bool		ModeError( int Status, int Mode );

	public:

				// Retunr true if file is already loaded, otherwise false;
				bool		Loaded( std::string FilePath );

				// Load all .script files in Path
				bool		LoadLUAFiles( std::string Path = LUA_SCRIPTS_PATH );

				bool		ExposeCppObjects();

				// Load as a module the file, with Name as name if specified
				bool		LoadFile( std::string FilePath, std::string Name = "" );

				bool		ExecuteFile( std::string FilePath );

				// Call a function, in a class\table if specified
				bool		CallFunction ( const char *Space, const char *Name, int NumArg = 1 );
		
				// Push a user type value
				bool		PushUserdata( void *Value, std::string Type, bool IsUserdata = true, bool bForced = false );

				// Try retrieve a user type
				template < typename T >
				T *			GetUserdata( int Index, bool bGeneric = false );
	
};

// Public
// bGeneric = Define if a metatable check or a simple type coversione must be done
template < typename T >
T *cScriptEngine::GetUserdata( int Index, bool bGeneric ) {
	luaL_checktype( L, Index, LUA_TUSERDATA );

	void *pValue = NULL;
	if ( !bGeneric ) {
		std::string ManagledName( typeid( T ).name() );
		ManagledName = ManagledName.substr( 6 );
		const char* TypeID = ManagledName.c_str();

		pValue = luaL_checkudata( L, Index, TypeID );
		if ( !pValue ) cMsgCRT( "cScriptEngine::GetUserdata:NULL value for data type %s ", TypeID );
	} else {
		pValue = lua_touserdata( L, Index );
		if ( !pValue ) cMsgCRT( "cScriptEngine::GetUserdata:NULL value" );
	}
	return ( pValue ) ? ( ( T * ) ( *( ( void** ) pValue ) ) ) : NULL;
}