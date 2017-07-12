
#include "World.h"


cWorld *pWorld = 0;

cWorld::cWorld() {
	
	cSection *pSection = Settings.GetSection( "Quality" );

	iLightQuality			= L_CLAMP( pSection->GetInt( "LightQuality", 1 ), 0, 3 );
	iTerrainQuality			= L_CLAMP( pSection->GetInt( "TerrainQuality", 1 ), 0, 2 );
	iTessellationQuality	= L_CLAMP( pSection->GetInt( "TessellationQuality", 1 ), 0, 3 );
	iWaterQuality			= L_CLAMP( pSection->GetInt( "WaterQuality", 1 ), 0, 3 );
	iTextureQuality			= L_CLAMP( 2 -pSection->GetInt( "TextureQuality", 2 ), 0, 2 );

	// 0 = Max, 1 = Mid, 2 = Low
	Leadwerks::Texture::SetDetail( iTextureQuality );
	bIsOK = true;
}


cWorld::~cWorld() {
//	SAFE_DELETE ( pMainWorld );
	cMsgDBG( "World destroyed.." );
}

bool cWorld::Initialize() {

	if ( !bIsOK ) return false;
	
	cMsgDBG( "cWorld::Initialize()" );
	
	cMsgDBG( "cWorld::Initialized" );
	bIsInitialized = true;
	return true;
}

bool cWorld::Create() {

	if ( !bIsInitialized ) {
		cMsgCRT( "cWorld::Create:Cannot create world if not initialized first" );
		return false;
	}

	if ( !pMainWorld && !( pMainWorld = Leadwerks::World::Create() ) ) {
		cMsgCRT( "cWorld::Create:Cannot create main world" );
		return false;
	}


	pMainWorld->SetLightQuality( iLightQuality );
	pMainWorld->SetTerrainQuality( iTerrainQuality );
	pMainWorld->SetWaterQuality( iWaterQuality );
	pMainWorld->SetTessellationQuality( iTessellationQuality );
	pMainWorld->SetPhysicsDetail( 1 );		// Default
	pMainWorld->Clear();

	return true;
}


bool cWorld::PickAtPosition( Leadwerks::Vec3 vPos, Leadwerks::PickInfo &Info, const float fRadius, const int fCType ) {
	return pMainWorld->Pick( vPos, vPos + 0.000000000000000000001, Info, fRadius, false, fCType );

}

void cWorld::Update() {

	Leadwerks::Window* Window = Engine()->Renderer()->GetWindow();
	if ( Window->Active() ) {
		Leadwerks::Time::Resume();
		
		// Update the World.
		pMainWorld->Update();

//		Life().Update();
	} else {
		Leadwerks::Time::Pause();
	}

	// per evitare che le armi "entrino nei modelli di fronte al giocatore
	// potrei use un world pick a breve distanza, e nel caso colpisca qualcosa
	// abbassare l'arma


}

void cWorld::Render( void ) {
	pMainWorld->Render();
}

