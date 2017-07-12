
#pragma once

#include <Engine.h>

#include "World.h"
#include "Sector.h"
#include "ProjectileManager\ProjectileManager.h"
#include "TaskManager\TaskManager.h"
#include "WeatherManager\WeatherManager.h"
#include "Difficulty\Difficulty.h"

#include "SpawnInfo.h"
#include "Game\Entities\Entity.h"
#include "Game\Entities\LiveEntities\Humans\Actor\\Actor.h"

#define MAX_TIME_FACTOR 3.0
#define MAX_ENTIIES_PER_LEVEL 500

class cLevel {
	private:
		// Entites
		std::vector < cEntity * >				vEntities;
		std::vector < cSpawnInfo * >			vSpawnData;
		std::vector < cSector * >				vSectors;

		unsigned long int ID					= ZERO_INT;
		
		cProjectileManager* pProjectileManager	= NULL;
		cWeatherManager*	pWeatherManager		= NULL;
		cTaskManager*		pTaskManager		= NULL;
		cDifficulty*		pDifficulty			= NULL;

		std::string sName						= "";
	public:	std::string sMapPath;

	private:
		float fTimeFactor						= 1.0;

		bool bIsPresent							= false;
		bool bIsInitialized						= false;
	
	public:
		cLevel();
		~cLevel();
				bool				Initialize();
		inline	bool				IsInitialized()			{ return bIsInitialized; }
		inline	bool				IsPresent()				{ return bIsPresent; }
		inline	void				SetPresent()			{ bIsPresent = true; }
				bool				Reset();
		
		inline	cProjectileManager*	ProjectileManager()			{ return pProjectileManager; }
		inline	cWeatherManager *	WeatherManager()			{ return pWeatherManager; }
		inline	cTaskManager *		TaskManager()				{ return pTaskManager; }
		inline	cDifficulty *		Difficulty()				{ return pDifficulty; }

		inline	unsigned long int	NewID()						{ ID++; return ID; }

		inline	void				SetTimeFactor( float Val )	{ fTimeFactor = L_CLAMP( Val, 0.1, MAX_TIME_FACTOR );  };
		inline	float				GetTimeFactor()				{ return fTimeFactor; };
		
		inline	const std::string	Name() const				{ return sName; }

	private:
				bool				LoadSpawnData( std::string LevelName );
				bool				ProcessSpawn();

	public:
				bool				Load( std::string FileName );
				cEntity	*			ProcessInfos( cEntity * Entity, const std::list < std::string > linfos );
		
		inline	void				AddSpawnInfo( cSpawnInfo* p ) { if ( p ) vSpawnData.push_back( p ); }
				cEntity *			SpawnWithInfo( cSpawnInfo* SpawnInfo );
				cEntity *			Spawn( std::string SectionName, Leadwerks::Vec3 Position, Leadwerks::Vec3 Rotation, Leadwerks::Vec3 Scale );

				cSector *			FindSector( std::string Name );

		inline	cActor *			GetActor();
		inline	cEntity *			GetActorAsEntity();
				cEntity *			FindByID( unsigned long int ID = 0 );
				cEntity *			FindByName( std::string Name = "" );
				bool				Release( unsigned long int ID = 0 );
		
				void				Update();

				void				Destroy();
	
};

extern cLevel *pLevel;

inline cLevel* Level();
inline cLevel* Level() { if ( !pLevel ) pLevel = new cLevel(); return ( pLevel ); }

extern cEntity *pActor;


#include "Level_Inline.h"