
#pragma once

#include "Leadwerks.h"
#include "AudioSrc.h"

#define DEFAULT_FADE 2000

class cAudioDevice {
	private:

		std::map < std::string, cAudioSrc * >			mAudioSources;
		typedef std::pair < std::string, cAudioSrc * >	PairAudioSrc;

		Leadwerks::Listener *pListener					= NULL;
		cAudioSrc *pActualMusic							= NULL;
		float fMainVolume, fMusicVolume, fSoundsVolume;

		unsigned long iID								= 0;
		inline unsigned long NewID() { iID++; return iID; }

		bool bIsOK = false;
		bool bIsInitialized = false;

	public:
		cAudioDevice();
		~cAudioDevice();
				bool	Initialize();
		inline	bool	IsOK() { return bIsOK; }
		inline	bool	IsInitialized() { return bIsInitialized; }
				void	DBG_WriteFile( void );

				bool	LoadSources( void );
				void	Clear();

				void	Update();

		// Volumes
		inline	void	SetMainVolume( float fVal )		{ fMainVolume	= Leadwerks::Math::Clamp( fVal, 0.0f, 1.0f ); };
		inline	float	GetMainVolume( void )			{ return fMainVolume; };

		inline	void	SetMusicVolume( float fVal )	{ fMusicVolume	= Leadwerks::Math::Clamp( fVal, 0.0f, 1.0f );; };
		inline	float	GetMusicVolume( void )			{ return fMusicVolume; };

		inline	void	SetSoundsVolume( float fVal )	{ fSoundsVolume	= Leadwerks::Math::Clamp( fVal, 0.0f, 1.0f );; };
		inline	float	GetSoundsVolume( void )			{ return fSoundsVolume; };

		inline	Leadwerks::Listener *GetListener()		{ return pListener; }

				cAudioSrc *	cAudioDevice::FindSource( std::string Name );

				cAudioSrc *	LoadSource( const std::string FilePath );

				void		Stop( std::string Name );
				void		Stop( cAudioSrc *pSource );

				cAudioSrc *	Play2D( std::string Name, bool bLoop = false );
				cAudioSrc *	Play2D( cAudioSrc *pSource, bool bLoop = false );

				cAudioSrc *	Play3D( std::string Name, Leadwerks::Vec3 vPos, bool bLoop = false );
				cAudioSrc *	Play3D( cAudioSrc *pSource, Leadwerks::Vec3 vPos, bool bLoop = false );

				cAudioSrc *	PlayMusic( std::string Name, bool bLoop = false );
				cAudioSrc *	PlayMusic( cAudioSrc *pSource, bool bLoop = false );

				cAudioSrc *	FadeIn( std::string Name, int iMS = DEFAULT_FADE, bool bLoop = false );
				cAudioSrc *	FadeIn( cAudioSrc *pSource, int iMS = DEFAULT_FADE, bool bLoop = false );

				cAudioSrc *	FadeOut( std::string Name, int iMS = DEFAULT_FADE );
				cAudioSrc *	FadeOut( cAudioSrc *pSource, int iMS = DEFAULT_FADE );
				void		FadeOut( int iMS = DEFAULT_FADE );
};