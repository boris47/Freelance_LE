
#include "Audio.h"

#include "..\Utils\Utils.h"
#include "..\Utils\LTXReader\LTXReader.hpp"

#include <cstdio>
#include <iostream>
#include <vector>

/*
per trovare od usare un oggetto AudioSrc immettere la stringa partendo direttamente dal nome della directory
subito prossima a "Sounds" o "Musics" oppure se non in una directory più specifica direttamente il nome del file
Es: in Sounds "Steps.Default.Default1"
Es: in Musics "final_theme"

//	cAudioSrc *p = pAudioDevice->FindSource( "final_theme" );
//	if ( p ) p->FadeIn(5000)->SetVolume( 0.5 );
*/


// Constructor
cAudioDevice::cAudioDevice() {

	cMsgDBG("cAudioDevice::cAudioDevice()");

	cSection *pSection = Settings.GetSection( "Audio" );

	fMainVolume		= L_CLAMP( pSection->GetFloat( "MainVolume", 1.0 ), 0.0f, 1.0f );
	fMusicVolume	= L_CLAMP( pSection->GetFloat( "MusicVolume", 1.0 ), 0.0f, 1.0f );
	fSoundsVolume	= L_CLAMP( pSection->GetFloat( "SoundsVolume", 1.0 ), 0.0f, 1.0f );

	pListener = Leadwerks::Listener::Create();

	cMsgDBG( "cAudioDevice::cAudioDevice() OK" );
	bIsOK = true;
}

// Destructor
cAudioDevice::~cAudioDevice() {

	this->Clear();

}

// Public
bool cAudioDevice::Initialize() {

	if ( !bIsOK ) {
		cMsgCRT( "cAudioDevice::Initialize:cAudioDevice bad initialization" );
		return false; // leave bIsInitialized == false
	}

	cMsgDBG("cAudioDevice::Initialize()");
	
	
	cMsgDBG( "cAudioDevice::Initialized" );
	bIsInitialized = true;
	return true;
}

const char EOF = -1;
#include <fstream>

// Public
void cAudioDevice::DBG_WriteFile() {

	std::ofstream Audio_Resources( "Audio_Resources.txt", std::ofstream::out );
	for ( const auto pPair: mAudioSources ) {

		Audio_Resources << pPair.second->Path() << " = " << pPair.first << "\n";
	}

	Audio_Resources.close();

}


// Public
bool cAudioDevice::LoadSources() {

	if ( !bIsInitialized ) return false;

	for ( auto FilePath : ListFiles( MUSIC_FILES_PATH, ".ogg" ) ) {
		cAudioSrc *pSource = LoadSource( FilePath );
		if ( !pSource ) {
			cMsgCRT( "AudioDevice::LoadResources:Unable to load music file %s", FilePath.c_str() );
			this->Clear();
			return false;
		}
		
		pSource->SetVolume( fMainVolume * fMusicVolume );
		mAudioSources[ pSource->Path() ] = pSource;
	}
	
	for ( auto FilePath : ListFiles( SOUND_FILES_PATH, ".wav" ) ) {
		cAudioSrc *pSource = LoadSource( FilePath );
		if ( !pSource ) {
			cMsgCRT( "AudioDevice::LoadResources:Unable to load sound file %s", FilePath.c_str() );
			this->Clear();
			return false;
		}
		
		pSource->SetVolume( fMainVolume * fSoundsVolume );
		mAudioSources[ pSource->Path() ] = pSource;
	}

	return true;
}

// Public
void cAudioDevice::Clear() {

	for ( auto &pPair : mAudioSources ) {
		pPair.second->Source()->Stop();
		SAFE_DELETE( pPair.second );
	}
	mAudioSources.clear();

}

void cAudioDevice::Update() {

	if ( !bIsInitialized ) return;

	for ( auto &pPair : mAudioSources ) {
		( pPair.second )->Update();
	}
}

// Public
cAudioSrc *cAudioDevice::FindSource( std::string Name ) {

	if ( !bIsInitialized ) return NULL;
	
	for ( const auto &pPair : mAudioSources ) {
		if ( pPair.second->Name() == Name ) return pPair.second;
	}

	return NULL;
}

// Public
cAudioSrc *cAudioDevice::LoadSource( std::string FilePath ) {

	if ( !bIsInitialized ) return NULL;

	std::string Name = RemoveExtension( String_Transform( FilePath.substr( MUSIC_FILES_PATH.size() + strlen("\\") ) ) );
	if ( FindSource( Name.c_str() ) ) {
		cMsgCRT( "AudioDevice::LoadSource:File %s already loaded", FilePath.c_str() );
		return NULL;
	}

	cAudioSrc *pSource = new cAudioSrc( Name, FilePath, this );
	if ( !pSource->IsOK() ) {
		SAFE_DELETE( pSource );
		return NULL;
	}

	return pSource;

}

// Public
void cAudioDevice::Stop( std::string Name ) {

	if ( !bIsInitialized ) return;

	cAudioSrc *pSource = FindSource( Name );

	Stop( pSource );
}


// Public
void cAudioDevice::Stop( cAudioSrc *pSource ) {

	if ( !bIsInitialized ) return;

	if ( pSource ) pSource->Source()->Stop();

}

// Public
cAudioSrc *cAudioDevice::Play2D( std::string Name,bool bLoop ) {

	if ( !bIsInitialized ) return NULL;

	cAudioSrc *pSource = FindSource( Name );
	if ( !pSource ) {
		cMsgCRT( "cAudioDevice::Play2D:Source for file %s not found!!", Name.c_str() );
		return NULL;
	}

	return Play2D( pSource, bLoop );
}

// Public
cAudioSrc *cAudioDevice::Play2D( cAudioSrc *pSource, bool bLoop ) {

	if ( !bIsInitialized ) return NULL;
	
	if ( pSource ) pSource->Play2D();

	return pSource;
}

// Public
cAudioSrc *cAudioDevice::Play3D( std::string Name, Leadwerks::Vec3 vPos, bool bLoop ) {
	
	if ( !bIsInitialized ) return NULL;

	cAudioSrc *pSource = FindSource( Name );
	if ( !pSource ) {
		cMsgCRT( "cAudioDevice::Play3D:Source for file %s not found!!", Name.c_str() );
		return NULL;
	}

	return Play3D( pSource, vPos, bLoop );
}

// Public
cAudioSrc *cAudioDevice::Play3D( cAudioSrc *pSource, Leadwerks::Vec3 vPos, bool bLoop ) {
	
	if ( !bIsInitialized ) return NULL;

	if ( pSource ) pSource->Play3D( vPos, bLoop );

	return pSource;
}

// Public
cAudioSrc *cAudioDevice::PlayMusic( std::string Name, bool bLoop ) {
	
	if ( !bIsInitialized ) return NULL;

	cAudioSrc *pSource = FindSource( Name );
	if ( !pSource ) {
		cMsgCRT( "cAudioDevice::PlaymMusic:Source for file %s not found!!", Name.c_str() );
		return NULL;
	}

	return PlayMusic( pSource, bLoop );
}

// Public
cAudioSrc *cAudioDevice::PlayMusic( cAudioSrc *pSource, bool bLoop ) {

	if ( !bIsInitialized ) return NULL;

	if ( pSource ) {
		pSource->Reset(); // remove every fade effect and restor 100% volume

		if ( pSource->IsPlaying() ) pSource->Source()->Stop();
		pSource->Play2D( bLoop );
		pActualMusic = pSource;
	}

	return pSource;
}


// Public
cAudioSrc *cAudioDevice::FadeIn( std::string Name, int iMS, bool bLoop ) {

	if ( !bIsInitialized ) return NULL;
	return FadeIn( FindSource( Name ), iMS, bLoop );

}

// Public
cAudioSrc *cAudioDevice::FadeIn( cAudioSrc *pSource, int iMS, bool bLoop ) {

	if ( !bIsInitialized ) return NULL;
	if ( pSource ) pSource->FadeIn( iMS, bLoop );
	return pSource;

}

// Public
cAudioSrc *cAudioDevice::FadeOut( std::string Name, int iMS ) {

	if ( !bIsInitialized ) return NULL;
	cAudioSrc *pSource = FindSource( Name );
	return FadeOut( pSource, iMS );

}

// Public
cAudioSrc *cAudioDevice::FadeOut( cAudioSrc *pSource, int iMS ) {

	if ( !bIsInitialized ) return NULL;
	if ( pSource ) pSource->FadeOut( iMS );
	return pSource;

}

// Public
void cAudioDevice::FadeOut( int iMS ) {

	if ( !bIsInitialized || !pActualMusic ) return;
	pActualMusic->FadeOut( iMS );

}
