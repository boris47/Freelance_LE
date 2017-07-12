#pragma once

#include <Engine.h>
#include "Level.h"

class cWorld {

	// Mondo nel quale creare tutto l'ambiente
	Leadwerks::World* pMainWorld	= NULL;
	
	int iLightQuality				= ZERO_INT;
	int iTerrainQuality				= ZERO_INT;
	int iTessellationQuality		= ZERO_INT;
	int iWaterQuality				= ZERO_INT;
	int iTextureQuality				= ZERO_INT;

	bool bIsOK						= false;
	bool bIsInitialized				= false;
	
	public:
		cWorld();
		~cWorld();
				bool Initialize();
		inline	bool IsOK()							{ return bIsOK; }
		inline	bool IsInitialized()				{ return bIsInitialized; }

		inline	const int GetLightQuality()			const { return iLightQuality; }
		inline	const int GetTerrainQuality()		const { return iTerrainQuality; }
		inline	const int GetTessellationQuality()	const { return iTessellationQuality; }
		inline	const int GetWaterQuality()			const { return iWaterQuality; }
		inline	const int GetTextureQuality()		const { return iTextureQuality; }

		inline	Leadwerks::World *					GetWorld() { return pMainWorld; }

				bool Create();

				bool PickAtPosition( Leadwerks::Vec3 vPos, Leadwerks::PickInfo &Info, const float fRadius = 0.0f, const int fCType = 0 );
		
				void Update();

				void Render();
	
};

extern cWorld *pWorld;

cWorld *World();
inline cWorld *World() { if ( !pWorld ) pWorld = new cWorld(); return ( pWorld ); }