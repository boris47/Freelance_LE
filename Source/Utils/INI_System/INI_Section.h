

#pragma once

class cINI_Section {
	private:
		std::string sName = "";

		std::unordered_map < std::string, std::string > mSectionMap;
		typedef std::unordered_map < std::string, std::string > SectionMap;

	public:
		cINI_Section( std::string Name, cINI_Section * Inherited );
		~cINI_Section();

		// Return section name
		inline const std::string	Name() { return sName; }

		// Try parse string and set raw key and value if exist
		inline bool					AddLine( std::string Line );

		// Directly add key and value in section map, for a valid key of course ( len > 0 :) )
		inline bool					AddKeyValue( std::string Key, std::string Value );

		// Return a copy of internal section map
		inline SectionMap			GetData( void );

		// Return section size ( usually equals line of section )
		inline size_t				Lines( void ) { return mSectionMap.size(); }

		// Return true is section contains that key
		inline bool					HasKey( std::string Key );

		// return true if Key exists and set his val in Value
		inline bool					GetValue( std::string Key, std::string &Value );

		// Return the raw value of entire initial line, if exists
		inline std::string			GetRawLine	( std::string Key, std::string	Default = std::string( "" ) );

		inline int					GetInt		( std::string Key, int			Default = -1 );
		inline bool					bGetInt		( std::string Key, int &Out,	int Default = -1 );

		inline bool					GetBool		( std::string Key, bool			Default = false );
		inline bool					bGetBool	( std::string Key, bool &Out,	bool Default = false );

		inline float				GetFloat	( std::string Key, float		Default = -1.0 );
		inline bool					bGetFloat	( std::string Key, float &Out,	float Default = -1.0 );

		inline std::string			GetString	( std::string Key, std::string	Default = "" );
		inline bool					bGetString	( std::string Key, std::string &Out, std::string Default = "" );

		inline bool					IsMultivalue( std::string Key );


		inline size_t				GetMultiSize( std::string Key );
		template < typename T >
			   bool					GetMultiValue( std::string Key, T &out, size_t Index, int Type );
		inline Leadwerks::Vec2		GetVec2		( std::string Key );
		inline Leadwerks::Vec3		GetVec3		( std::string Key );
		inline Leadwerks::Vec4		GetVec4		( std::string Key );

		
		inline bool					bGetMultiValue( std::string Key, std::string &Out, size_t Index, int Type );
		inline bool					bGetVec2	( std::string Key, Leadwerks::Vec2 &Out, Leadwerks::Vec2 Default = { 0.0, 0.0 } );
		inline bool					bGetVec3	( std::string Key, Leadwerks::Vec3 &Out, Leadwerks::Vec3 Default = { 0.0, 0.0, 0.0 } );
		inline bool					bGetVec4	( std::string Key, Leadwerks::Vec4 &Out, Leadwerks::Vec4 Default = { 0.0, 0.0, 0.0, 0.0 } );

		inline void					SetInt		( std::string Key, int Value );
		inline void					SetBool		( std::string Key, bool Value );
		inline void					SetFloat	( std::string Key, float Value );
		inline void					SetString	( std::string Key, std::string Value );
		inline void					SetValue	( std::string Key, std::string Value );
		inline void					SetMultiValue( std::string Key, std::vector < std::string > vValues );
		inline void					SetVec2		( std::string Key, Leadwerks::Vec2 &Vec );
		inline void					SetVec3		( std::string Key, Leadwerks::Vec3 &Vec );

};

#include "INI_Section_inline.h"
