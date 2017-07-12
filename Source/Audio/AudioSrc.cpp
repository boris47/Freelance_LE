
#include "Audio.h"
#include "AudioSrc.h"
#include "..\Engine.h"

// Constructor
cAudioSrc::cAudioSrc( const std::string Name, const std::string FilePath, cAudioDevice *pDevice ) {
	
	sName = Name;
	sFilePath = FilePath;
	this->pDevice = pDevice;

	Leadwerks::Sound *pSound = Leadwerks::Sound::Load( FilePath );
	if ( !pSound ) {
		cMsgCRT( "cAudioSrc:Constructor:Cannot load sound " + FilePath );
		return; // leave bIsOK == false
	}

	pSource = Leadwerks::Source::Create();
	pSource->SetSound( pSound );
	pSound->Release();
	bIsOK = true;

}

// Destructor
cAudioSrc::~cAudioSrc() {
//	SAFE_RELEASE( pSource );
}


// Public
void cAudioSrc::Update() {

	if ( bToFade ) {
		long iDeltaTime = iFadeTime - Leadwerks::Time::GetCurrent();
		float fOperation = float( iDeltaTime ) / float( iTime );

		if ( iDeltaTime <= 0 ) {
			bToFade = false;
			if ( bFadeIn ) {
				fVolumeCoef = 1.0f;
			} else {
				fVolumeCoef = 0.0f;
				pSource->Stop();
			}
			fVolumeCoef = (bFadeIn) ? 1.0f : 0.0f;
		}
		
		fVolumeCoef = (bFadeIn)?( 1 - fOperation ): fOperation;;
	}

	pSource->SetVolume( fVolume * fVolumeCoef );

}


// Public
void cAudioSrc::Reset( void ) {

	iFadeTime = iTime = 0;
	bToFade = bFadeIn = false;
	fVolumeCoef = 1.0f;

}


// Public
cAudioSrc *cAudioSrc::SetVolume( float fVal ) {

	float fMainVolume = pDevice->GetMainVolume();

	if ( fMainVolume == ZERO_FLOAT ) {
		pSource->Stop();
		return this;
	}

	fVolume = fVal;
	pSource->SetVolume( fMainVolume * fVolume * fVolumeCoef );

	return this;

}

// Public
float cAudioSrc::GetVolume( void ) {

	return pSource->GetVolume();

}


// Public
cAudioSrc *cAudioSrc::SetPitch( float fVal ) {

	pSource->SetPitch( fVal );
	return this;

}

// Public
float cAudioSrc::GetPitch( void ) {

	return pSource->GetPitch();

}

// Public
cAudioSrc *cAudioSrc::FadeIn( int iMS, bool bLoop ) {

	if ( iMS <= 0 ) return NULL;

	Reset();

	bToFade = true; bFadeIn = true;
	pSource->SetVolume( 0.0f );
	pSource->SetLoopMode( bLoop );
	pSource->Play();
	iFadeTime = Leadwerks::Time::GetCurrent() + iMS;
	iTime = iMS;
	return this;

}

// Public
void cAudioSrc::FadeOut( int iMS ) {

	if ( iMS <= 0 ) return;
	bToFade = true; bFadeIn = false;
	iFadeTime = Leadwerks::Time::GetCurrent() + iMS;
	iTime = iMS;

}

// Public
cAudioSrc *cAudioSrc::Play2D( bool bLoop ) {

	if ( IsPlaying() ) pSource->Stop();
	pSource->SetLoopMode( bLoop );
	pSource->Play();
	return this;

}

// Public
cAudioSrc *cAudioSrc::Play3D( Leadwerks::Vec3 vPos, bool bLoop ) {

	if ( IsPlaying() ) pSource->Stop();
	pSource->SetPosition( vPos );
	pSource->SetLoopMode( bLoop );
	pSource->Play();
	return this;

}