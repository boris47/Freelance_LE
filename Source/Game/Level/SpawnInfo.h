
#pragma once

#include <Engine.h>

class cSpawnInfo {
	std::string sSection		= "";
	Leadwerks::Vec3 vPosition	= EMPTY_VEC3;
	Leadwerks::Vec3 vRotation	= EMPTY_VEC3;
	Leadwerks::Vec3 vScale		= EMPTY_VEC3;	
	typedef std::list < std::string > InfoList;
	InfoList lInfos; // can contain an Info, LuaFunction
	
	public:
		cSpawnInfo();
		cSpawnInfo ( std::string Section, Leadwerks::Vec3 Position = EMPTY_VEC3, Leadwerks::Vec3 Rotation = EMPTY_VEC3, Leadwerks::Vec3 Scale = EMPTY_VEC3 );
		
		inline std::string			GetSection()							{ return sSection; };
		inline Leadwerks::Vec3		GetPosition()							{ return vPosition; };
		inline Leadwerks::Vec3		GetRotation()							{ return vRotation; };
		inline Leadwerks::Vec3		GetScale()								{ return vScale; };

		inline void					SetSection( std::string s )				{ this->sSection = s; };
		inline void					SetPosition( Leadwerks::Vec3 v )		{ this->vPosition = v; };
		inline void					SetRotation( Leadwerks::Vec3 v )		{ this->vRotation = v; };
		inline void					SetScale( Leadwerks::Vec3 v )			{ this->vScale = v; };
		inline InfoList				GetInfoLine()							{ return lInfos; };

		// Spawn infos
		bool		HasNoInfo();
		void		SetInfo( std::string Info );
		void		AddInfo( std::string Info );
		bool		FindInfo( std::string Info );
		std::string GetInfo( std::string Info );
		std::string GetInfoValue( std::string Info );
		void		SubInfo( std::string Info );
	
};