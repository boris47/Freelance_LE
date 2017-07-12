
#include "ScriptEngine.h"
#include "ScriptExtension.h"


#include "Exports\Export_Actor.h"
#include "Exports\Export_AudioDevice.h"
#include "Exports\Export_Camera.h"
#include "Exports\Export_Camera_Effects.h"
#include "Exports\Export_Engine.h"
#include "Exports\Export_Entity.h"
#include "Exports\Export_FlashLight.h"
#include "Exports\Export_Human.h"
#include "Exports\Export_InputManager.h"
#include "Exports\Export_Inventory.h"
#include "Exports\Export_Level.h"
#include "Exports\Export_LiveEntity.h"
#include "Exports\Export_LTXReader.h"
#include "Exports\Export_Utils.h"
#include "Exports\Export_WeatherManager.h"



#include "Leadwerks.h"
#include "Utils\Utils.h"

#include <stdio.h>
#include <iostream>
const char EOF = -1;
#include <fstream>
#include <streambuf>

#define LUA_LOAD_MODE 1
#define LUA_EXEC_MODE 2

std::string LUA_GetMetatableName( const std::string Type ) {
	return std::string( Type ).append( "_mt" );
}

bool RaiseError( lua_State *L, const char *ErrMsg ) {
	cMsgCRT( "cScriptEngine:Error: %s: %s", ErrMsg, lua_tostring( L, -1 ) );
	lua_settop( L, 0 );
	return true;
}

void LUA_PrintStack( lua_State *L ) {

	if ( lua_gettop( L ) == 0 ) return cMsgDBG( "EMPTY LUA STACK\n" );
	cMsgDBG( "C - LUA STACK START" );

	for ( int i = 1; i < 8; i++ )	cMsgDBG( "%2d : %s", i, lua_typename( L, lua_type( L, i ) ) );
	for ( int i = 1; i < 8; i++ )	if ( lua_isstring( L, i ) ) cMsgDBG( "--: Slot %d: %s", i, lua_tostring( L, i ) );
	cMsgDBG( "C - LUA STACK END\n" );

}

static int LUA_AtPanicFunction( lua_State *L ) {

	cMsgCRT( lua_tostring( L, lua_gettop( L ) ) );
	Engine()->QuitRequest();
	return 0;

}


// Constructor
cScriptEngine::cScriptEngine() {
	if ( !Leadwerks:: Interpreter::L ) Leadwerks::Interpreter::Reset();
	L = Leadwerks:: Interpreter::L;

	lua_atpanic( L, LUA_AtPanicFunction );

	// As tolua does:
	// EMPTY STACK
	lua_settop( L, 0 );
	lua_pushstring( L, "_Script_ObjectBox_" );				// String
	lua_newtable( L );										// String, Table1
	{
		/* make weak value metatable for ubox table to allow userdata to be garbage-collected */
		lua_newtable( L );									// String, Table1, Table2
		lua_pushliteral( L, "__mode" );						// String, Table1, Table2, String
		lua_pushliteral( L, "v" );							// String, Table1, Table2, String, String
		lua_rawset( L, 3 );									// String, Table1, Table2
	}
	lua_setmetatable( L, 2 );								// String, Table1
	lua_rawset( L, LUA_REGISTRYINDEX );						// EMPTY STACK

	lua_settop( L, 0 );										// Ensure EMPTY STACK

	LUA_RegisterExtensions( L );

	bIsOK = true;
}


// Private
bool cScriptEngine::ModeError( int Status, int Mode ) {

	if ( Status == 0 ) return false;

	if ( Mode == LUA_LOAD_MODE ) {
		switch ( Status ) {
			case LUA_ERRSYNTAX:	{ return RaiseError( L, "[LUA-C] Syntax error during pre-compilation" ); };
			case LUA_ERRMEM:	{ return RaiseError( L, "[LUA-C] Memory allocation error" ); };
			case LUA_ERRFILE :	{ return RaiseError( L, "[LUA-C] Cannot open/read the file" ); };
		}
	} else if ( Mode == LUA_EXEC_MODE) {
		switch ( Status ) {
			case LUA_ERRRUN:	{ return RaiseError( L, "[LUA-C] A runtime error has occurred" ); };
			case LUA_ERRMEM:	{ return RaiseError( L, "[LUA-C] A memory allocation error has occurred" ); };
			case LUA_ERRERR:	{ return RaiseError( L, "[LUA-C] Error while running the error handler function" ); };
		}
	}

	return false;
}


// Public
bool cScriptEngine::Loaded( std::string FilePath ) {

	return ( std::find( lLoaded.begin(), lLoaded.end(), FilePath ) != lLoaded.end() ) ? true : false;

}


// Public
bool cScriptEngine::LoadLUAFiles( std::string Path ) {

	for ( std::string FilePath : ListFiles( Path, ".script" ) ) {
		if ( !LoadFile( FilePath ) ) return false;
	}

	return true;

}


// Public
bool cScriptEngine::ExposeCppObjects() {

	if ( !LUA_Expose_Actor( L )				) return false;
	if ( !LUA_Expose_AudioDevice( L )		) return false;
	if ( !LUA_Expose_CamManager( L )		) return false;
	if ( !LUA_Expose_Camera_Effects( L )	) return false;
	if ( !LUA_Expose_Engine( L )			) return false;
	if ( !LUA_Expose_Entity( L )			) return false;
	if ( !LUA_Expose_FlashLight( L )		) return false;
	if ( !LUA_Expose_Human( L )				) return false;
	if ( !LUA_Expose_InputManager( L )		) return false;
	if ( !LUA_Expose_Inventory( L )			) return false;
	if ( !LUA_Expose_Level( L )				) return false;
	if ( !LUA_Expose_LiveEntity( L )		) return false;
	if ( !LUA_Expose_LTXReader( L )			) return false;
	if ( !LUA_Expose_Utils( L )				) return false;
	if ( !LUA_Expose_WeatherManager( L )	) return false;

	return true;
}


// Public
bool cScriptEngine::LoadFile( std::string FilePath, std::string Name ) {
	
	if ( this->Loaded( FilePath ) ) {
		cMsgCRT( "cScriptEngine::Load:File \"%s\" already loaded!", FilePath.c_str() );
		return false;
	}

	std::string FileName = GetNameFromFilePath( FilePath );
	std::ifstream ScriptFile( FilePath.c_str(), std::ifstream::in );
	std::string Buffer((std::istreambuf_iterator< char >( ScriptFile ) ), std::istreambuf_iterator< char >() );
	ScriptFile.close();
	
	std::string sName ( ( Name.length() > 0 ) ? ( ("\"") + Name + ("\"") ) : "..." );
	std::string Script = ( "module(" + sName + ", function(t)setmetatable(t, {__index = _G});end)\n" )
		.append( "local function ScriptName() return\"" + FileName + "\";end\n" )
		.append( Buffer );


	int iOldTop = lua_gettop( L );

	int Status = luaL_loadbuffer(L, Script.c_str(), Script.length(), FilePath.c_str() );// Function
	if ( ModeError( Status, LUA_LOAD_MODE ) || !lua_isfunction( L, -1 ) ) {
		cMsgCRT( "cScriptEngine::LoadFile:Load of module \"%s\" failed", FilePath.c_str() );
		lua_settop( L, iOldTop );
		return false;
	}
	
	lua_pushstring( L, FileName.c_str() );												// Function, ModName
	Status = lua_pcall( L, 1, 1, 0 );													// Value
	if ( ModeError( Status, LUA_EXEC_MODE ) ) {
		lua_settop( L, iOldTop );
		return false;
	}

	lua_settop( L, iOldTop );
	lLoaded.push_back( FilePath );
	return true;
}


bool cScriptEngine::ExecuteFile( std::string FilePath ) {
	
	// if fails
	if ( luaL_dofile( L, FilePath.c_str() ) ) {
		cMsgCRT( lua_tostring( L, -1 ) );
		return false;
	}
	return true;
}


// Public
bool cScriptEngine::CallFunction( const char *Space, const char *Name, int NumArg ) {
	
	if ( !Name ) {
		cMsgCRT( "cScriptEngine::CallFunction: invalid function name " );
		return false;
	}

	int Lua_Top = lua_gettop( L );

	lua_getglobal( L, "_G" );															// ..., _G
	if ( Space ) {
		lua_getfield( L, -1, Space );													// ..., _G, Module
		if ( !lua_istable( L, -1 ) ) {
			cMsgCRT( "cScriptEngine:Module %s doesn't exist", Space );
			return false;
		}
			
		lua_getfield( L, -1, Name );													// ..., _G, Module, Function
		if ( lua_isnil( L, -1 ) ) {
			cMsgCRT( "cScriptEngine:Module \"%s\":function \"%s\" doesn't exist", Space, Name );
			return false;
		}
	} else {
		lua_getfield( L, -1, Name );													// ..., _G, Function
		if ( lua_isnil( L, -1 ) ) {
			cMsgCRT( "cScriptEngine:Function \"%s\" doesn't exist", Name );
			return false;
		}
	}
	
	if ( NumArg > 0 ) {
		for ( int idx = ( Lua_Top - NumArg ); idx < NumArg; idx++ ) {
			if ( lua_type( L, idx ) != LUA_TNONE ) lua_pushvalue( L, idx );
		}
	}
	
	int Status = lua_pcall( L, NumArg, 0, 0 );
	if ( ModeError( Status, LUA_EXEC_MODE ) ) {
		lua_settop( L, Lua_Top );
		return false;
	}
	
	lua_settop( L, Lua_Top - NumArg );
	return true;
}


// Public
bool cScriptEngine::PushUserdata( void *Value, std::string Type, bool IsUserdata, bool bForced ) {
	
	if ( !Value ) {
		cMsgCRT( "[ LUA ]: Pushing %s data fail", Type.c_str() );
		lua_pushnil( L );
		return false;
	}

	if ( !IsUserdata ) {
		lua_pushlightuserdata( L, Value );
		return true;
	}

	luaL_getmetatable( L, Type.c_str() );									// ..., Metatable
	if ( !lua_istable( L, -1 ) ) {
		luaL_error( L, "cScriptEngine::PushUserdata:Trying to push userdata with invalid metatable, %s", Type.c_str() );
		lua_pop( L, 1 );													// ...
		lua_pushnil( L );													// ..., Nil
		return false;
	}

//	As tolua do:
	lua_pushstring( L, "_Script_ObjectBox_" );								// ..., Metatable, String
	lua_rawget( L, LUA_REGISTRYINDEX );										// ..., Metatable, MainTable

	lua_pushlightuserdata( L, Value );										// ..., Metatable, MainTable, Pointer
	lua_rawget( L, -2 );													// ..., Metatable, MainTable, Value!?

	if ( lua_isuserdata( L, -1 ) ) {										// ..., Metatable, MainTable, Userdata

		// when forced, force metatable change
		if ( !bForced ) {
			lua_getmetatable( L, -1 );										// ..., Metatable, MainTable, Userdata, MetaT
			if ( lua_rawequal( L, -1, -4 ) ) {								// Metatables are equal
				lua_pop( L, 1 );											// ..., Metatable, MainTable, Userdata
			} else {														// ..., NewMT, MainTable, Userdata, OldMT
				lua_pop( L, 1 );											// ..., NewMT, MainTable, Userdata
				lua_pushvalue( L, -3 );										// ..., NewMT, MainTable, Userdata, NewMT
				lua_setmetatable( L, -2 );									// ..., NewMT, MainTable, Userdata
			}
		} else {
			lua_pushvalue( L, -3 );											// ..., Metatable, MainTable, Userdata, Metatable
			lua_setmetatable( L, -2 );										// ..., Metatable, MainTable, Userdata
		}

		lua_remove( L, -2 );												// ..., Metatable, Userdata
		lua_remove( L, -2 );												// ..., Userdata
		return true;
	}

	// ..., Metatable, MainTable, Non Userdata
	// Arrivato fin qui, dato uso solo io quella table, si tratta di dover creare un nuovo oggetto

	lua_pop( L, 1 );													// ..., Metatable, MainTable

	lua_pushlightuserdata( L, Value );									// ..., Metatable, MainTable, Pointer
	*( void ** ) lua_newuserdata( L, sizeof( void * ) ) = Value;		// ..., Metatable, MainTable, Pointer, Value

	lua_insert( L, -4 );												// ..., Value, Metatable, MainTable, Pointer
	
	lua_pushvalue( L, -4 );												// ..., Value, Metatable, MainTable, Pointer, Value
	
	lua_rawset( L, -3 );												// ..., Value, Metatable, MainTable
	lua_pop( L, 1 );													// ..., Value, Metatable
	lua_setmetatable( L, -2 );											// ..., Value
	
	return true;
}