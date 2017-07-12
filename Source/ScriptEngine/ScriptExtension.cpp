

#include "ScriptExtension.h"
#include "..\Utils\Utils.h"
#include "..\Utils\LTXReader\LTXReader.hpp"


const char EOF = -1;
#include <fstream>

std::ofstream LUA_File( "LUA_Exports.txt", std::ofstream::out );

/*************************/
/**  My Lua Class Start	**/
/*************************/


unsigned char CheckKeyWords( lua_State *L ) {
	const char *KeyWord = lua_tostring( L, 3 );								// uClassMTMother, uClassMTChild, Key, Value
	if ( KeyWord[ 0 ] == '_' && KeyWord[ 1 ] == '_' ) return 0;
	if ( strcmp( KeyWord, "Initialize" )	== 0 ) return 0;
	if ( strcmp( KeyWord, "Finalize" )		== 0 ) return 0;
	return 1;
}


static int __BindMotherClass( lua_State *L ) {								// Val
	if ( !lua_isstring( L, 1 ) && !lua_isuserdata( L, 1 ) )
		return luaL_error( L, "Userdata or string expected for child from mother class constructor" );
	
	if ( lua_isstring( L, 1 ) )	{											// String?
		const char *ClassName = lua_tostring( L, 1 );
		lua_pop( L, 1 );
		lua_getglobal( L, ClassName );										// uClassMother?
		if ( !lua_isuserdata( L, 1 ) )										// uClassMother
		return luaL_error( L, "No class have name %s", ClassName );
	}
	
	if ( !lua_getmetatable( L, 1 ) )										// uClassMother, uClassMotherMT
		return luaL_error( L, "This userdata has not a metatable" );
	lua_remove( L, 1 );														// uClassMotherMT
	
	lua_getfield( L, 1, "__Name" );											// uClassMotherMT, String?
	const char *ClassName = lua_tostring( L, 2 );
	if ( ClassName == NULL )												// uClassMotherMT, String!
		return luaL_error( L, "This userdata has not a valid metatable" );
	lua_pop( L, 1 );														// uClassMotherMT
	
	lua_pushvalue( L, lua_upvalueindex( 1 ) );								// uClassMotherMT, uClassChild
	
	lua_getmetatable( L, 2 );												// uClassMotherMT, uClassChild, uClassChildMT
	lua_remove( L, 2 );														// uClassMotherMT, uClassChildMT
	
//	Mother = 1, Child = 2, Key = 3, Value = 4;
	lua_pushnil( L );														// uClassMTMother, uClassMTChild, Nil
	while( lua_next( L, 1 ) != 0 )	{										// uClassMTMother, uClassMTChild, Key, Value
		if ( CheckKeyWords( L ) ) {
			lua_pushvalue( L, 3 );											// uClassMTMother, uClassMTChild, Key, Value, Key
			lua_insert( L, 4 );												// uClassMTMother, uClassMTChild, Key, Key, Value
			lua_rawset( L, 2 );												// uClassMTMother, uClassMTChild, Key
		} else lua_pop( L, 1 );												// uClassMTMother, uClassMTChild, Key
	}
	return 0;
}


static int _GCFunction( lua_State *L ) {									// uClass
	lua_getfield( L, 1, "Finalize" );
	if ( lua_isfunction( L, -1 ) ) {										// uClass, Function
		lua_pushvalue( L, 1 );												// uClass, Function, uClass
		int Error = lua_pcall( L, 1, 0, 0 );								// 
		if ( Error && !lua_isnil( L, -1 ) ) {
			MessageBox( NULL, lua_tostring( L, -1 ), "LUA Error!", MB_ICONEXCLAMATION | MB_OK );
			return 0;
		}
	} else lua_pop( L, 1 );													// uClass
	
	lua_getmetatable( L, 1 );												// uClass, uClassMT
	lua_getfield( L, 2, "__Name" );											// uClass, uClassMT, String
	const char *ClasseName = lua_tostring( L, 3 );								// uClass, uClassMT, String
	lua_pop( L, 2 );														// uClass
	lua_gc( L, LUA_GCCOLLECT, 0 );
	lua_getglobal( L, "_G" );												// uClass, _G
	lua_pushstring( L, ClasseName );										// uClass, _G, String
	lua_rawget( L, 2 );														// uClass, _G, sName
	lua_pushnil( L );														// uClass, _G, sName, Nil
	lua_rawset( L, 2 );														// uClass
	return 0;
}


static int _CallFunction( lua_State *L ) {									// uClass, ...
	lua_getfield( L, 1, "__initialized" );									// uClass, ..., Bool
	if ( !lua_toboolean ( L, -1 ) ) {										// uClass, ..., False?
		lua_pop( L, 1 );													// uClass, ...
		lua_pushboolean( L, 1 );											// uClass, ..., True
		lua_setfield( L, 1, "__initialized" );								// uClass, ...
		
		lua_getfield( L, 1, "Initialize" );									// uClass, ..., Value
		
		if ( lua_isfunction( L, -1 ) ) {									// uClass, ..., Function?
			lua_insert( L, 2 );												// uClass, Function, ...
			lua_pushvalue( L, 1 );											// uClass, Function, ..., uClass
			lua_insert( L, 3 );												// uClass, Function, uClass, ...
			int Error = lua_pcall( L, lua_gettop( L ) - 2, LUA_MULTRET, 0 );// uClass, ...
			if ( Error && !lua_isnil( L, -1 ) ) {
				MessageBox( NULL, lua_tostring( L, -1 ), "LUA Error!", MB_ICONEXCLAMATION | MB_OK );
				return 0;
			}
		} else lua_pop( L, 1 );
	} else lua_pop( L, 1 );
	return lua_gettop( L );
}


static int luaB_class( lua_State *L ) {
	const char *ClassName = luaL_checkstring( L, 1 );						// String
	lua_settop( L, 0 );
	if ( !luaL_newmetatable( L, ClassName ) ) {								// MT
		return luaL_error( L, "[LUA]:The class \"%s\" already exists", ClassName );
	}
	
	int MyMtTable = lua_gettop( L );
	
	lua_pushliteral( L, "__index" );
	lua_pushvalue( L, MyMtTable );
	lua_rawset( L, MyMtTable );												// MT
	
	lua_pushliteral( L, "__newindex" );
	lua_pushvalue( L, MyMtTable );
	lua_rawset( L, MyMtTable );												// MT
	
	lua_pushliteral( L, "__Name" );
	lua_pushstring( L, ClassName );
	lua_rawset( L, MyMtTable );												// MT
	
	lua_pushliteral( L, "__call" );
	lua_pushcfunction( L, _CallFunction );
	lua_rawset( L, MyMtTable );												// MT
	
	lua_pushliteral( L, "__gc" );
	lua_pushcfunction( L, _GCFunction );
	lua_rawset( L, MyMtTable );												// MT
	
	lua_newuserdata( L, sizeof( void * ) );									// MT, uClass
	lua_insert( L, 1 );														// uClass, MT
	
	lua_setmetatable( L, 1 );												// uClass
	lua_pushvalue( L, 1 );													// uClass, uClass
	lua_setglobal( L, ClassName );											// uClass
	
	lua_pushcclosure( L, __BindMotherClass, 1 );							// uClass
	return 1;
}





/*************************/
/**  My Lua Class End	**/
/*************************/






bool Lua_Constant( lua_State *L, std::string Name, double Val ) {			// ..., Table

	if ( !L || !Name.length() || ( !lua_istable( L, -1 ) ) ) {
		return false;
	}

	lua_pushstring( L, Name.c_str() );										// ..., Table, String
	lua_pushnumber( L, Val );												// ..., Table, String, Number
	lua_rawset( L, -3 );													// ..., Table

	return true;

}


bool LUA_Class_Open( lua_State *L, std::string ClassName, lua_CFunction pCallFunction, lua_CFunction pGCFunction ) {

	if ( !L || !ClassName.length() ) return false;

	int iLuaTop = lua_gettop( L );

	lua_getglobal( L, ClassName.c_str() );									// ..., Something
	if ( !lua_isnil( L,-1 ) ) {												// is something identitied by that name
		cMsgCRT( "[C]LUA_Class:The class \"%s\" already exists", ClassName.c_str() );
		// leave something in the stack, all methods registerations fail and when closing,
		// something will be finally removed, so stack remain the same
		return false;
	}
	lua_settop( L, iLuaTop );												// ...

	std::string MT_Name = "c" + ClassName;
	luaL_getmetatable( L, MT_Name.c_str() );								// ..., Something
	if ( !lua_isnil( L, 1 ) ) {
		cMsgCRT( "[C]LUA_Class:Metatable %s already exists", MT_Name.c_str() );
		return false;
	}
	lua_settop( L, iLuaTop );												// ...

	lua_newuserdata( L, sizeof( void * ) );									// ..., uClass

	lua_newtable( L );														// ..., uClass, Metatable

	if ( pCallFunction ) {
		lua_pushliteral( L, "__call" );										// ..., uClass, Metatable, __call
		lua_pushcfunction( L, pCallFunction );								// ..., uClass, Metatable, __call, cfunc
		lua_rawset( L, -3 );												// ..., uClass, Metatable

		if ( pGCFunction ) {
			lua_pushliteral( L, "__gc" );									// ..., uClass, Metatable, __gc
			lua_pushcfunction( L, pGCFunction );							// ..., uClass, Metatable, __gc, cfunc
			lua_rawset( L, -3 );											// ..., uClass, Metatable
		}
	}

	lua_setmetatable( L, -2 );												// ..., uClass
	lua_setglobal( L, ClassName.c_str() );									// ...

	LUA_File << "Class " + ClassName + " Metatable: c" + ClassName;
	if ( pCallFunction ) LUA_File << "  with constructor";
	LUA_File << "\n";
	
	// Prepare metatable for lua userdata
	luaL_newmetatable( L, MT_Name.c_str() );								// ..., Table
	lua_pushstring( L, "__index" );											// ..., Table, String
	lua_pushvalue( L, -2 );													// ..., Table, String, Table
	lua_rawset( L, -3 );													// ..., Table

	lua_pushstring( L, "__newindex" );										// ..., Table, String
	lua_pushvalue( L, -2 );													// ..., Table, String, Table
	lua_rawset( L, -3 );													// ..., Table

	return true;
}


bool LUA_Class_Register( lua_State *L, std::string Namespace, std::string FuncName, lua_CFunction pFunction, std::string sReturned, std::string sDescription ) {

	// Only valid arguments accepted
	if ( !FuncName.length() || !pFunction ) {								// ..., Table
		cMsgCRT( "LUA_Class_Register:Invalid function name or function" );
		return false;
	}

	// Override is not supported
	lua_pushstring( L, FuncName.c_str() );									// ..., Table, String
	lua_rawget( L, -2 );
	if ( lua_isfunction( L, -1 ) ) {										// ..., Table, Function ?
		cMsgCRT( "Namespace %s has already function %s defined", Namespace.c_str(), FuncName.c_str() );
		lua_pop( L, 2 );													// ...
		return false;
	}
	lua_pop( L, 1 );														// ..., Table

	// Finally register our function
	lua_pushstring( L, FuncName.c_str() );									// ..., Table, String
	lua_pushcfunction( L, pFunction );										// ..., Table, String, Function
	lua_rawset( L, -3 );													// ..., Table

	LUA_File << "    " + FuncName + "( " + sDescription + " ) = " + sReturned + "\n";
	return true;
}

void LUA_Class_Close( lua_State *L ) {
	lua_pop( L, 1 );
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

static int lua_Metatable_As( lua_State *L ) {

	luaL_checktype( L, 1, LUA_TUSERDATA );
	luaL_checktype( L, 2, LUA_TSTRING );									// Class, String
	std::string MT_Name = lua_tostring( L, 2 );

	luaL_getmetatable( L, ( "c" + MT_Name ).c_str() );						// Class, String, MT
	if ( !lua_istable( L, 3 ) ) {
		return luaL_error( L, "cannot get metatable for type %s", MT_Name.c_str() );
	}

	lua_remove( L, 2 );														// Class, MT

	lua_pushstring( L, "_Script_ObjectBox_" );								// Class, MT, String
	lua_rawget( L, LUA_REGISTRYINDEX );										// Class, MT, MainTable

	lua_pushvalue( L, 2 );													// Class, MT, MainTable, MT
	lua_rawget( L, 3 );														// Class, MT, MainTable, ??

	if ( lua_isuserdata( L, 4 ) ) {
		lua_insert( L, 1 );													// Userdata, Class, MT, MainTable
		lua_pop( L, 3 );													// Userdata
		return 1;
	}

	lua_pop( L, 1 );														// Class, MT, MainTable

	void *Value = *( void ** ) lua_touserdata( L, 1 );
	*( void ** ) lua_newuserdata( L, sizeof( void * ) ) = Value;			// Class, MT, MainTable, Userdata

	lua_insert( L, 1 );														// Userdata, Class, MT, MainTable

	lua_pushvalue( L, 3 );													// Userdata, Class, MT, MainTable, MT
	lua_pushvalue( L, 1 );													// Userdata, Class, MT, MainTable, MT, Userdata;
	lua_rawset( L, 4 );														// Userdata, Class, MT, MainTable

	lua_pop( L, 1 );														// Userdata, Class, MT

	lua_remove( L, 2 );														// Userdata, MT

	lua_setmetatable( L, 1 );												// Userdata
	return 1;
	
}

bool LUA_MetaTable_Open( lua_State *L, std::string Type ) {

	if ( !L || !Type.length() ) return false;

	if ( !luaL_newmetatable( L, Type.c_str() ) ) {							// ..., Table
		cMsgCRT( "Metatable %s already exists", Type.c_str() );
		return false;
	}

	lua_pushstring( L, "__index" );											// ..., Table, String
	lua_pushvalue( L, -2 );													// ..., Table, String, Table
	lua_rawset( L, -3 );													// ..., Table

	lua_pushstring( L, "__newindex" );										// ..., Table, String
	lua_pushvalue( L, -2 );													// ..., Table, String, Table
	lua_rawset( L, -3 );													// ..., Table

	lua_pushstring( L, "As" );												// ..., Table, String
	lua_pushcfunction( L, lua_Metatable_As );								// ..., Table, String, Function
	lua_rawset( L, -3 );													// ..., Table

	LUA_File << "Methods for object " + Type + "\n";
	return true;

};

bool LUA_MetaTable_Register( lua_State *L, std::string FuncName, lua_CFunction pFunction, std::string sReturned, std::string sDescription ) { // ..., Table

	if ( !pFunction || !FuncName.length() || !lua_istable( L, -1 ) ) {
		cMsgCRT( "LUA_MetaTable_Register:Invalid function name or function" );
		return false;														// now return always false
	}

	lua_pushstring( L, FuncName.c_str() );									// ..., Table, String
	lua_pushcfunction( L, pFunction );										// ..., Table, String, Function
	lua_rawset( L, -3 );													// ..., Table

	LUA_File << "    " + FuncName + "( " + sDescription + " ) = "+ sReturned + "\n";
	return true;

}

void LUA_MetaTable_Close( lua_State *L ) {
	lua_pop( L, 1 );
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

bool LUA_NameSpace_Open( lua_State *L, std::string Name ) {

	if ( !L || !Name.length() ) return false;

	lua_getglobal( L, Name.c_str() );										// ..., Something
	if ( !lua_isnil( L, 1 ) ) {												// is something identitied by that name
		cMsgCRT( "[C]LUA_NameSpace_Open:Object named \"%s\" already exists", Name.c_str() );
		return false;
	}

	lua_newtable( L );														// ..., Table
	lua_pushvalue( L, -1 );													// ..., Table, Table
	lua_setglobal( L, Name.c_str() );										// ..., Table
	
	LUA_File << "Namespace " + Name + "\n";
	return true;
}


void LUA_RegisterExtensions( lua_State *L ) {
	
	cMsgDBG( "Registering class Function" );

	lua_register( L, "class", luaB_class );

	cMsgDBG( "Registration OK" );


}
