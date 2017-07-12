
#pragma once


#define FILE_SETTINGS			"Settings.ltx"
#define FILE_GAMEDATA			"Gamedata.pak"
#define FILE_SAVE				"SaveFile.dat"

#ifdef LAST_LE_VER
	#define FILE_LTX			".\\Config\\All.ltx"
#else
	#define FILE_LTX			"Config\\All.ltx"
#endif

#if defined( LAST_LE_VER )
	#define CONFIG_FILES_PATH		std::string( ".\\Config" )
	#define SPAWN_FILES_PATH		std::string( ".\\Spawn" )
	#define MUSIC_FILES_PATH		std::string( ".\\Musics" )
	#define SOUND_FILES_PATH		std::string( ".\\Sounds" )
	#define LUA_SCRIPTS_PATH		std::string( ".\\Scripts" )
	#define MAP_FILES_PATH			std::string( ".\\Maps" )
#else
	#define CONFIG_FILES_PATH		std::string( "Config" )
	#define SPAWN_FILES_PATH		std::string( "Spawn" )
	#define MUSIC_FILES_PATH		std::string( "Musics" )
	#define SOUND_FILES_PATH		std::string( "Sounds" )
	#define LUA_SCRIPTS_PATH		std::string( "Scripts" )
	#define MAP_FILES_PATH			std::string( "Maps" )
#endif

#define ON						"1"
#define OFF						"0"

// USEFULL IN HEADER FILES FOR PRE SET VARIABLES

#define ZERO_FLOAT				0.0f
#define ONE_FLOAT				1.0f

#define ZERO_INT				0
#define ONE_INT					1


#define EMPTY_VEC2				Leadwerks::Vec2( ZERO_FLOAT )
#define EMPTY_VEC3				Leadwerks::Vec3( ZERO_FLOAT )
#define EMPTY_VEC4				Leadwerks::Vec4( ZERO_FLOAT )
#define EMPTY_STRING			std::string( "" )

#define SAFE_JUST_DELETE( p )	{ if ( p ) { delete ( p ); } }
#define SAFE_DELETE( p )		{ if ( p ) { delete ( p );     ( p ) = nullptr; } }
#define SAFE_DELETE_ARRAY( p )	{ if ( p ) { delete[] ( p );   ( p ) = nullptr; } }
#define SAFE_RELEASE( p )		{ if ( p ) { ( p )->Release(); SAFE_DELETE( p ); } }
#define SAFE_RETURN( p )		{ if ( p ) { return p; } }
#define SAFE_PTR_RETURN( p )	{ if ( p ) return p; else return nullptr; }

#define GET_AS( p, T )			dynamic_cast<T*>(p)
#define CONVERT_TO( p, T )		static_cast<T*>(p)