
#pragma once

#include <Leadwerks.h>

// Forward declaration
class cAudioDevice;

class cAudioSrc {
	private:
		Leadwerks::Source *pSource	= nullptr;
		cAudioDevice *pDevice		= nullptr;
		
		float fVolumeCoef			= 1.0f;
		float fVolume				= 1.0f;

		std::string sName			= "";
		std::string sFilePath		= "";

		int iFadeTime				= 0;
		int iTime					= 0;

		bool bToFade				= false;
		bool bFadeIn				= false;
		bool bIsOK					= false;

	public:
		cAudioSrc( const std::string Name, const std::string FilePath, cAudioDevice *pDevice );
		~cAudioSrc();
		inline	bool					IsOK( void ) { return bIsOK; }

		inline	std::string				Name( void ) { return sName; }
		inline	std::string				Path( void ) { return sFilePath; }
		inline	Leadwerks::Source *		Source( void ) { return pSource; }

		inline	bool					IsPlaying( void ) { return pSource->GetState() == Leadwerks::Source::Playing; }

				void					Update( void );

				void					Reset( void );
		
				cAudioSrc *				SetVolume( float fVal );
				float					GetVolume( void );

				cAudioSrc *				SetPitch( float fVal );
				float					GetPitch( void );

				cAudioSrc *				FadeIn( int iMS, bool bLoop = false );
				void					FadeOut( int iMS );

				cAudioSrc *				Play2D( bool bLoop = false );
				cAudioSrc *				Play3D( Leadwerks::Vec3 vPos, bool bLoop = false );
		

		

};