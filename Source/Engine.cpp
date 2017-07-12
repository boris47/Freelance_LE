
#include <cstdio>
#include <fstream>

#include "Engine.h"
#include "Game\Level\Level.h"
#include "Game\Level\World.h"
#include "Game\Camera\CamManager.h"

cEngine *pEngine = 0;

const std::string sStartlevelName = "start";

// Constructor
cPeriodicFunction::cPeriodicFunction( long int Time, HookFunction fn, HookCondition cond ) {

	if ( !fn ) return;

	iID					= Engine()->NewID();
	iExecuteTime		= Time;
	iTimer				= Leadwerks::Time::GetCurrent() + iExecuteTime;
	pFunction			= fn;
	pCondition			= cond;
	bIsOK				= true;

}


// Public
cEngine::cEngine() {

	bIsOK = true;

}


// Public
cEngine::~cEngine() {

	this->Reset();
	cMsgDBG( "Engine destroyed.." );

}

inline void LUA_FixedUpdate() {

	if ( !pWorld || !pWorld->GetWorld() ) return;

	auto v = World()->GetWorld()->entities;

	std::for_each( v.begin(), v.end(), [&] ( auto &p ) {
		if ( p->ContainsFunction( "FixedUpdate" ) )
			p->CallFunction( "FixedUpdate" );
	} );

}

// Public
bool cEngine::Initialize() {

	this->iInternalState = EngineStates::LAUNCH;

	if ( !bIsOK ) {
		cMsgCRT( "cEngine::Initialize:bad initialization" );
		return false; // leave bIsInitialized == false
	}

	////////////////////////////////////////////////////////////////////////////////////////
	cMsgDBG( "cEngine::Initialize()" );
	////////////////////////////////////////////////////////////////////////////////////////
	// Script Engine
	{
		if ( !( pScriptEngine = new cScriptEngine() ) || !pScriptEngine->IsOK() ) {
			cMsgCRT( "cEngine::Constructor:Cannot start script engine!" );
			return false; // leave bIsInitialized == false
		}
		pScriptEngine->ExecuteFile( LUA_SCRIPTS_PATH + "\\" + "Test.lua" );

		if ( !pScriptEngine->ExposeCppObjects() || !pScriptEngine->LoadLUAFiles( LUA_SCRIPTS_PATH ) ) {
			return Reset(); // leave bIsInitialized == false
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Settings
	{
		// If dont have a settings file create one
		if ( !FileExists( FILE_SETTINGS ) ) PrepareSettingsFile();
		if ( !( pLTXSettings = new cLTXReader( FILE_SETTINGS ) ) || !pLTXSettings->IsOK() ) {
			cMsgCRT( "cEngine::Constructor:LTXReader cannot read settings file, closing!" );
			return Reset(); // leave bIsInitialized == false
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Video Device
	{
		if ( !( pRenderer = new cRenderer() ) || !pRenderer->Initialize() ) {
			return Reset(); // leave bIsInitialized == false
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Audio Device
	{
		if ( !( pAudioDevice = new cAudioDevice() ) || !pAudioDevice->Initialize() ) {
			return Reset(); // leave bIsInitialized == false
		}
		pAudioDevice->LoadSources();

	}
	
	////////////////////////////////////////////////////////////////////////////////////////
	// Custom World Class
	{
		if ( !World()->IsOK() || !World()->Initialize() ) {
			return Reset(); // leave bIsInitialized == false
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// LTX Reader
	{
		if ( !( pLTXReader = new cLTXReader( FILE_LTX ) ) || !pLTXReader->IsOK() ) {
			cMsgCRT( "cEngine::Initialize:LTXReader not initialized because an error, closing!" );
			return Reset(); // leave bIsInitialized == false
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Instance cLevel object, Create cWeatherManager object
	{
		if ( !Level()->Initialize() ) {
			return Reset(); // leave bIsInitialized == false
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Internals
	{
		iUpdateTime = Leadwerks::Time::GetCurrent();
		pInputManager = new cInputManager();
//		pUI_Manager = new cUI_Manager();
//		pUI_Manager->Initialize();

	}

	Engine()->RegisterPeriodicHook( 1000 / 60, LUA_FixedUpdate );

	cMsgDBG( "cEngine::Initialized" );

//	pUI_Manager->ShowMainMenu();

	bIsInitialized = true;
	return true;
}

// Public
bool cEngine::Reset() {

	vPeriodicHooks.clear();
	SAFE_DELETE( pUI_Manager );
	SAFE_DELETE( pInputManager );
	SAFE_DELETE( pWorld );

	SAFE_DELETE( pLTXReader );
	SAFE_DELETE( pScriptEngine );
	SAFE_DELETE( pAudioDevice );
	SAFE_DELETE( pRenderer );
	SAFE_DELETE( pLTXSettings );

	return false;
}

// Private
void cEngine::PrepareSettingsFile() {
	ofstream SettingsFile;
	SettingsFile.open( FILE_SETTINGS );
	////////////////////////////////////////////////////////////////////////////////////////
	// Video
	{
		std::string sVideo = std::string( "[Video]\n" )
			.append( "Width                 = 1024; \n" )
			.append( "Height                = 768;\n" )
			.append( "Fullscreen            = false;\n" )
			.append( "Vsync                 = true;\n\n" );
		SettingsFile << sVideo;
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Quality
	{
		std::string sQuality = std::string( "[Quality]\n" )
			.append( "; Values are 0 = Low, 1 = Medium, 2 = High, 3 = Maximum\n" )
			.append( "LightQuality          = 3; Light Quality, 0, 1, 2, 3\n" )
			.append( "SunQuality            = 3; Quality of the sun ( 0, 1, 2, 3 )\n" )
			.append( "TerrainQuality        = 0; Terrain Resoultion, ( 0, 1, 2 ) Maximum is 2\n" )
			.append( "TessellationQuality   = 0; Tessellation, 0, 1, 2, 3\n" )
			.append( "WaterQuality          = 0; Water Quality, 0, 1, 2, 3\n\n" )
			.append( "TextureQuality        = 2; Texture Quality, ( 0, 1, 2 ) Maximum is 2" );

		SettingsFile << sQuality;
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Camera
	{
		std::string sCamera = std::string( "[Camera]\n" )
			.append( "FOV                   = 70.0; The player field of view\n" )
			.append( "Bloom                 = true; Show the bloom effect\n" )
	//		.append( "SSDO                  = true; Screen Space Ambient Directional Effect\n" )
			.append( "FXAA                  = true; Fast Approximate Anti-Aliasing\n" )
			.append( "DynamicWeather        = true;  If set true enable dynamic weather cycle\n" )
			.append( "MouseSensitivity      = 2.2;\n" )
			.append( "MultiSample           = 1;    Multisampling (MSAA) = 1, 2, 4, 8, 16 ( Zero is considere as 1 )\n\n" );
		SettingsFile << sCamera;
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Effects
	{
		std::string sEffects = std::string( "[Effects]\n" )
			.append( "HeadBob               = true; The bob effect while moving\n" )
			.append( "HeadMove              = true; The movement to simulate breathing\n" )
			.append( "HeadShake             = true; The shake on some triggers( nades, punch, ... )\n\n" );
		SettingsFile << sEffects;
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Audio
	{
		std::string sAudio = std::string( "[Audio]\n" )
			.append( "MainVolume            = 1.0;\n" )
			.append( "MusicVolume           = 1.0;\n" )
			.append( "SoundsVolume          = 0.6;\n" );
		SettingsFile << sAudio;
	}
	SettingsFile.close();
}

// Public
// TODO move this function on New Game of Main Menu
bool cEngine::StartWorld() {

	if ( !bIsInitialized ) return false;

	cMsgDBG( "cEngine::StartWorld()" );

	/*	LOAD ORDER
		-- Create WeatherManager, initialize it, set time
		-- Create Level, Load spawnData, create World, load map, process spawn, load weather
		-- Create Camera, create all camera effects, load postprocesses
		-- Setup Callback System
	*/

	// Load all spawn data, create world, load map and load weather, spawn all entities by spawn data
	if ( !Level()->Load( sStartlevelName ) ) return false;

	Sleep( 500 );

	return true;
}


// Public
void cEngine::DrawInfo( char *format, ... ) {

	if ( !bIsInitialized ) return;

	char buffer[ 1024 ];
	{
		va_list arg;
		va_start( arg, format );
		vsnprintf( buffer, sizeof( buffer ), format, arg );
		va_end( arg );
	}
	vInfoQueue.push_back( std::string( buffer ) );

}


/*	??????????????????????????????????????????????????????????????????????????????????????????????????????
	Leadwerks UpdatePhysic one or more timer per frame
	Leadwerks UpdateWorld once per frame
???	EnginE::Update() This function update as fast as program run ???
----------------------------------------------------------------------------------------------------------
	For camera the best solution should be using UpdatePhysics taking last 2-3 result
	and using UpdateWorld to interpolate them

	For player movement you can multiply movements by TIME::GETSPEED() [ DELTA TIME FROM LAST FRAME ]

	Gradual transition variables can be curved by same value ( MATH::CURVE( A, B, X * TIME::GETSPEED() ) )

	UPDATE
	At last camera rotation and position are calculated every physic update, same for player movements with inputs
	because of "secure x times at second call", to make this functions frame rate indipendent

	UpdateWorld occurs every engine loop.

	curved variable must be processed in physic update function, letting different frame rate but same result

*///??????????????????????????????????????????????????????????????????????????????????????????????????????



// Public
bool cEngine::Loop() {

	if ( !bIsInitialized || bNeedToQuit ) return false;

	Leadwerks::Window* Window = pRenderer->GetWindow();
	if ( Window->Closed() || Window->KeyHit( Leadwerks::Key::Escape ) ) return false;
	////////////////////////////////////////////////////////////////////////////////////////
	// INPUT
	{
		// Update input signals
		pInputManager->Update();
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// UPDATE
	{
		
//		if ( this->GetInternalState() == EngineStates::INGAME ) {

			// This function updates a world.  Physics, particle emitters, navigation, and various other tasks will be updated,
			// and should be called once in the main loop
			if ( pWorld ) pWorld->Update();
		
			// Call my custom class updaters
			CallPeriodicHooks();

//		}

		// Update all playing audio sources
		pAudioDevice->Update();
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// RENDER
	{
		// Clean screen with black color
		pRenderer->Clear();
		
//		if ( this->GetInternalState() == EngineStates::INGAME ) {

			// This function renders the world
			if ( pWorld ) pWorld->Render();

//		}
		
//#if defined(_DEBUG)
		
		// Can draw usefull stats
		pRenderer->DrawStats();

///		DrawInfo( "LUA Usage: " + std::to_string( lua_gc( ScriptEngine()->GetState(), LUA_GCCOUNT, 0 ) ) + "KB" );
		
//#endif
		if ( Level()->IsPresent() ) {

			// Draw actor stamina pseudo bar
			std::string sStaminaLine = ".";
			float fStamina = Level()->GetActor()->GetStamina() * 100;
			for ( int i = 0; i < fStamina; i++ ) {
				if ( i == 20 ) { sStaminaLine.append( "!" ); continue; }
				if ( i == 30 ) { sStaminaLine.append( "!" ); continue; }
				sStaminaLine.append( "." );
			}
			DrawInfo( "STAMINA" );
			DrawInfo( sStaminaLine );

			// Draw actual day time as HH:MM:SS
			DrawInfo( Level()->WeatherManager()->GetActualTimeAsString() );

		}

		// Print all info pushed by the program
		DrawInfos();

		// Update UI elements
		pUI_Manager->Update();

		// Finally refresh the screen.
		pRenderer->Update();
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// COUNTERS
	{
		Leadwerks::Time::Update();
		Leadwerks::Math::SeedRandom( Leadwerks::Time::GetCurrent() );
	}
	return true;
}