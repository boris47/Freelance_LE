
#include "Level.h"
#include "Game\Camera\CamManager.h"


const char EOF = -1;
#include <fstream>


// Usefull for create a spawn file
//	int Count = 0;
//	std::ofstream outputFile("Level1.spawn");


/// <summary>MyMethod is a method in the MyClass class.  
/// <para>Here's how you could make a second paragraph in a description. <see cref="System::Console::WriteLine"/> for information about output statements.</para>  
/// <seealso cref="MyClass::MyMethod2"/>  
/// </summary>  
void ProcessEntity( Leadwerks::Entity *entity, Leadwerks::Object* extra ) {

	std::string sName = entity->GetKeyValue( "name" );

	if ( sName == "Player" ) {

		cSpawnInfo *pSpawnInfo = new cSpawnInfo();
		pSpawnInfo->SetSection( "Actor" );
		pSpawnInfo->SetPosition( entity->GetPosition() );
		pSpawnInfo->SetRotation( entity->GetRotation() );
		pSpawnInfo->SetScale( entity->GetScale() );

		Level()->AddSpawnInfo( pSpawnInfo );
		return;

	}

	if ( sName.find( "Sector" ) != std::string::npos ) {



	}

	// Remove every Direction Light
	// This let use one in the editor
	if ( entity->GetClass() == Leadwerks::Object::DirectionalLightClass ) {
		entity->Release();
		return;
	}

	// Remove every Camera
	if ( entity->GetClass() == Leadwerks::Object::CameraClass ) {
		entity->Release();
		return;
	}

	if ( entity->isprefab ) {
//		std::string section = GetNameFromFilePath( RemoveExtension( entity->prefabpath ) );
//		outputFile << ( "section=" + (section) + "\n" );
	} else {
//		outputFile << ( "section=" + entity->GetKeyValue( "name" ) + "\n" );
	}

	/*
	std::string pos_x = std::to_string(entity->position.x);
	std::string pos_y = std::to_string(entity->position.y);
	std::string pos_z = std::to_string(entity->position.z);
//	outputFile << ( "position=" + pos_x + "," + pos_y + "," + pos_z + "\n" );
	std::string dir_x = std::to_string(entity->rotation.x);
	std::string dir_y = std::to_string(entity->rotation.y);
	std::string dir_z = std::to_string(entity->rotation.z);
//	outputFile << ( "direction=" + dir_x + "," + dir_y + "," + dir_z + "\n" );
	std::string sca_x = std::to_string(entity->scale.x);
	std::string sca_y = std::to_string(entity->scale.y);
	std::string sca_z = std::to_string(entity->scale.z);
//	outputFile << ( "scale=" + sca_x + "," + sca_y + "," + sca_z + "\n" );

*/

}


cLevel *pLevel = 0;


// Constructor
cLevel::cLevel() {
	
	vEntities.clear();
	vSpawnData.clear();

	cSection *pSection = LTXReader.GetSection( "Time" );
	if ( pSection ) {
		fTimeFactor = pSection->GetFloat( "TimeFactor", 1.0 );
	} else cMsgCRT( "cLevel::Initialize:Cannot get time factor, default is 1.0" );

	// Preallocate enough memory
	vEntities.resize( MAX_ENTIIES_PER_LEVEL );

}


// Destructor
cLevel::~cLevel() {
	SAFE_DELETE( pProjectileManager );
	SAFE_DELETE( pWeatherManager );
	SAFE_DELETE( pTaskManager );
	SAFE_DELETE( pDifficulty );
	
	vEntities.clear();	// destroy all objects
	vSpawnData.clear();

}


// Public
bool cLevel::Initialize() {

	if ( bIsInitialized ) {
		cMsgCRT( "cLevel::Initialize:Already initialized!!!" );
		return false;
	}

	if ( !( pWeatherManager = new cWeatherManager() ) ) {
		cMsgCRT( "cLevel::Initialize:WeatherManager not avaiable" );
		SAFE_DELETE( pWeatherManager );
		return false; // leave bIsInitialized = false
	}

//	pTaskManager->OnInitialize();

	bIsInitialized = true;
	return true;
}


// Public
bool cLevel::Reset() {

	// Self reset
	vEntities.clear();	// destroy all objects
	vSpawnData.clear();

	// unload all weathers
	pWeatherManager->Reset();

	// clear all projectiles
//	pProjectileManager->Reset();

	bIsPresent = false;

	return false;
}


/*	LOAD ORDER
//-	CHECK PATHS,
//-	LOAD SPAWNDATA,
//-	CREATE WORLD,
//-	LOAD MAP,
//-	PROCESS SPAWN,
//-	LOAD & SET WEATHER
//-	CREATE & INITIALIZE & PARENT WITH ACTOR THE CAMERA, CREATE EFFECTS, LOAD POSTPROCESSES
//-	CREATE OR RESET ENGINE HOOK SYSTEM
*/
// Public
bool cLevel::Load( std::string LevelName ) {

	if ( !bIsInitialized ) return false;

	// if already loaded free previuos resources ( map change )
	if ( bIsPresent ) {
		this->Reset();
//		gamestate = changing map
//		load screen on;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//-	PATHS CHECK
	std::string sMapPath;
	std::string sLevelWheater;
	std::string sSkyMixerShader;
	{
		////////////////////////////////////////////////////////////////////////////////////////
		// Level Section
		cSection *pLevelSection = LTXReader.GetSection( LevelName );
		if ( !pLevelSection ) {
			cMsgCRT( "cLevel::Load:Cannot find section for level \"" + LevelName + "\"" );
			return this->Reset();
		}

		////////////////////////////////////////////////////////////////////////////////////////
		// Map Name
		std::string sMapName;
		if ( !pLevelSection->bGetString( "Map", sMapName ) ) {
			cMsgCRT( "cLevel::Load:Invalid map specified for level \"" + LevelName + "\"" );
			return this->Reset();
		}

		////////////////////////////////////////////////////////////////////////////////////////
		// Map file existence
		sMapPath = MAP_FILES_PATH + "\\" + sMapName + ".map";
		if ( !FileExists( sMapPath ) ) {
			cMsgCRT( "cLevel::Load:Cannot find file map \"" + sMapPath + "\"" );
			return this->Reset();
		}

		////////////////////////////////////////////////////////////////////////////////////////
		// Level weather specified
		if ( !pLevelSection->bGetString( "Weather", sLevelWheater ) ) {

			// If not speficied in config file, try to find deault weather
			if ( !LTXReader.GetSection( sLevelWheater ) ) {
				cMsgCRT( "cLevel::Load:Cannot find default weather section for level \"" + LevelName + "\", you must create one" );
				return this->Reset();
			}
			sLevelWheater = "Default";
		}

		////////////////////////////////////////////////////////////////////////////////////////
		// SkyMixer shader path
		if ( !pLevelSection->bGetString( "SkyMixer", sSkyMixerShader ) ) {
			cMsgCRT( "cLevel::Initialize:Cannot find file of \"SkyMixer\" for level \"" + LevelName + "\"" );
			return this->Reset();
		}

#ifdef LAST_LE_VER
		sSkyMixerShader = ".\\" + sSkyMixerShader;
#endif

		////////////////////////////////////////////////////////////////////////////////////////
		// SkyMixer file existence
		if ( !FileExists( sSkyMixerShader ) ) {
			cMsgCRT( "cLevel::Load:Cannot find file shader \"" + sSkyMixerShader + "\"" );
			return this->Reset();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//-	Load Entities Spawndata
	if ( !this->LoadSpawnData( LevelName ) ) return false;

	////////////////////////////////////////////////////////////////////////////////////////
	//-	Create Leadwerks World
	if ( !World()->Create() ) return false;

	////////////////////////////////////////////////////////////////////////////////////////
	//-	Load Level
	cMsgDBG( "cLevel::Load:Loading map " + sMapPath );
	if ( !Leadwerks::Map::Load( sMapPath, &ProcessEntity ) ) {
		cMsgCRT( "cLevel::Load:Error loading map " + sMapPath );
		return this->Reset();
	}
	cMsgDBG( "cLevel::Load: map " + sMapPath + " loaded." );
	bIsPresent = true; // Flag for level loaded

	////////////////////////////////////////////////////////////////////////////////////////
	//-	Spawn Entites
	if ( !this->ProcessSpawn() ) {
		return this->Reset();
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Initialize weather manager by creating effects ( Rain, Fog, Volumetric Light )
	if ( !pWeatherManager->Initialize() ) {
		return this->Reset();
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//- Create and initialize Camera
	if ( !CamManager()->Initialize( Level()->GetActor() ) ) {
		return this->Reset();
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//- Load & set weather
	if ( !pWeatherManager->Load( sLevelWheater, sSkyMixerShader ) ) {
		return this->Reset();
	}

	cSection *pCamSection = Settings.GetSection( "Camera" );
	pWeatherManager->SetDynamic( ( pCamSection ) ? pCamSection->GetBool( "DynamicWeather" ) : false );

	cMsgDBG( "Level \"%s\" loaded", LevelName.c_str() );

	return true;
}


// Private
bool cLevel::LoadSpawnData( std::string LevelName ) {

	if ( !bIsInitialized ) return false;

	std::string FilePath = std::string( SPAWN_FILES_PATH ).append( "\\" ).append( LevelName ).append( ".spawn" );
	if ( !FileExists( FilePath ) ) {
		cMsgCRT( "cLife::LoadSpawnData:File \"%s\" doesn't exist", FilePath.c_str() );
		return false;
	}
	
	std::ifstream FileHandler( FilePath.c_str() );
	if( !FileHandler.is_open() ) {	//if not opened, exit
		cMsgCRT( "cLife::LoadSpawnData:Cannot read \"%s\"", FilePath.c_str() );
		return false;
	}
	
	if ( FileHandler.peek() == std::ifstream::traits_type::eof() ) {
		cMsgCRT( "cLife::LoadSpawnData:Empty file \"%s\"", FilePath.c_str() );
		return false;
	}
	
	int iLine = 0;
	cSpawnInfo *SpawnInfo = NULL;
	while( FileHandler.good() ) {
		iLine++;
		std::string Line;
		std::getline( FileHandler, Line );

		if ( !String_IsValid( Line ) ) continue;
		
//		if ( Line.empty() ) continue;
		KeyValue_t KeyValue = GetKeyValue( Line );
		if ( KeyValue.IsOK ) {
			if ( KeyValue.Key == "Section" )	{ SpawnInfo->SetSection( KeyValue.Value ); continue; }
			if ( KeyValue.Key == "Position" )	{ SpawnInfo->SetPosition( StrToVec3( KeyValue.Value ) ); continue; }
			if ( KeyValue.Key == "Rotation" )	{ SpawnInfo->SetRotation( StrToVec3( KeyValue.Value ) ); continue; }
			if ( KeyValue.Key == "Scale" )		{ SpawnInfo->SetScale( StrToVec3( KeyValue.Value ) ); continue; }
			if ( KeyValue.Key == "Info" )		{ SpawnInfo->SetInfo( KeyValue.Value ); continue; }
		}
		
		if ( Line[ 0 ] && ( Line[ 0 ] == '[' ) ) {
			if ( SpawnInfo ) {
				vSpawnData.push_back( SpawnInfo );
			}
			SpawnInfo = new cSpawnInfo();
			continue;
		}

		cMsgCRT( "cLife::LoadSpawnData:Incorrect line %i in file %s", iLine, FilePath.c_str() );
		SAFE_DELETE( SpawnInfo );
		return false;
	}
	
	if ( SpawnInfo ) {
		vSpawnData.push_back( SpawnInfo );
	}
	return true;

}


// Private
bool cLevel::ProcessSpawn() {

	if ( !bIsInitialized ) return false;

	for ( auto pInfo : vSpawnData ) {
		
		if ( !this->SpawnWithInfo( pInfo ) ) {
			cMsgCRT( "cLevel::ProcessSpawn:Entity = NULL, section " + pInfo->GetSection() );
			return false;
		}

		cMsgDBG( "cLevel::ProcessSpawn:Processed spawn of " + pInfo->GetSection() );
	}

	return true;
}


// Public
cEntity *cLevel::ProcessInfos( cEntity * Entity, const std::list < std::string > linfos ) {
	return Entity;
}


// Public
cEntity *cLevel::SpawnWithInfo( cSpawnInfo* SpawnInfo ) {

	if ( !bIsInitialized || !bIsPresent ) return NULL;

	if ( !SpawnInfo ) return NULL;

	std::string SectionName = SpawnInfo->GetSection();
	Leadwerks::Vec3 Position = SpawnInfo->GetPosition();
	Leadwerks::Vec3 Rotation = SpawnInfo->GetRotation();
	Leadwerks::Vec3 Scale = SpawnInfo->GetScale();

	cEntity *Entity = this->Spawn( SectionName, Position, Rotation, Scale );
	if ( !Entity ) {
		cMsgCRT( "cLife::Create:Failed at creation of entity %s", SectionName.c_str() );
		return NULL;
	}

	ProcessInfos( Entity, SpawnInfo->GetInfoLine() );
	return Entity;
}



inline bool CreateActor( Leadwerks::Vec3 Position, Leadwerks::Vec3 Rotation ) {
	// Create Actor object
	cActor *p = new cActor( Position, Rotation );
	// if something go wrong delete object and return NULL
	if ( !p->IsOK() ) {
		SAFE_DELETE( p );
		return false;
	}

	// Set itself as userdata
	// ( This is used in lua to retrieve as cEntity from Leadwerks entity )
	p->GetEntity()->SetUserData( p );
	pActor = p; // As cEntity *

	return true;
}


// Public
cEntity *cLevel::Spawn( std::string SectionName, Leadwerks::Vec3 Position, Leadwerks::Vec3 Rotation, Leadwerks::Vec3 Scale ) {
	
	if ( !bIsInitialized || !bIsPresent ) return NULL;
	
	cSection * pSection = LTXReader.GetSection( SectionName );
	if ( !pSection ) return NULL;

	cEntity *pEntity = NULL;
	switch ( pSection->GetInt( "Type" ) ) {
// If the default statement is omitted, and no case match is found, none of the statements in the switch body are executed.	MSDN
		case ENTITY::Type::ACTOR		: {
				if ( !CreateActor( Position, Rotation ) ) return NULL;
				pEntity = pActor;
				break;
			
		}
//		case ENTITY::Type::ANIMAL	: {
//			pEntity = new cAnimal( Position, Rotation, Scale );
//			if ( pEntity->IsOK() ) break;
//			delete pEntity; break;
//		}
//		case ENTITY::Type::HUMAN		: {
//			pEntity = new cHuman( Position, Rotation, Scale );
//			if ( pEntity->IsOK() ) break;
//			delete pEntity; break;
//		}
//		case ENTITY::Type::OBJECT	: {
//			pEntity = new cObject( Position, Rotation, Scale );
//			if ( pEntity->IsOK() ) break;
//			delete pEntity; break;
//		}
	}
	if ( !pEntity ) return NULL;
	
	/*
//	Parse entity
	if ( Section->HasKey( "Prefab" ) ) { // compiled objects
		std::string sPrefab = Section->GetString( "Prefab" );
		if ( FileExists( sPrefab ) ) {
			Leadwerks::Entity *L_entity = Leadwerks::Prefab::Load( sPrefab ); // Load pfb file
			entity->SetEntity( L_entity );
		}
	}
	*/


	vEntities.push_back( pEntity );
	return pEntity;
}


// Public
cSector * cLevel::FindSector( std::string Name ) {

	for ( auto pSector : vSectors ) if ( pSector->Name() == Name  ) return pSector;
	return NULL;

}



// Public
cEntity * cLevel::FindByID( unsigned long int ID ) {

	if ( !bIsInitialized || !bIsPresent ) return NULL;
	if ( !ID || ID < 0 ) return NULL;

	for ( auto pEntity : vEntities ) {
		if ( pEntity->ID() == ID ) return pEntity;
	}

	return NULL;

}


// Public
cEntity *cLevel::FindByName( std::string Name ) {

	if ( !bIsInitialized || !bIsPresent ) return NULL;
	if ( !Name.length() ) return NULL;
	
	for ( auto pEntity : vEntities ) {
		if ( pEntity->Name() == Name ) return pEntity;
	}

	return NULL;
}


// Public
bool cLevel::Release( unsigned long int ID ) {

	if ( !bIsInitialized || !bIsPresent ) return NULL;
	if ( !ID || ID < 0 ) return NULL;

	for ( auto pEntity : vEntities ) {
		if ( pEntity->ID() == ID ) { delete pEntity; return true; }
	}

	return false;

}


// Public
void cLevel::Update() {

	if ( !bIsInitialized || !bIsPresent ) return;

	for ( auto pEntity : vEntities ) {
		
	}
}


// Public
void cLevel::Destroy() {
	SAFE_DELETE( pProjectileManager );
	SAFE_DELETE( pDifficulty );
	SAFE_DELETE( pTaskManager );
	SAFE_DELETE( pWeatherManager );
}