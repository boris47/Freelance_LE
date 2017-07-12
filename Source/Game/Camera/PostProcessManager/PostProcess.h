
#pragma once

#include <Engine.h>

class cPostProcess {
	private:
		// Time fade apply
		long iFadeTime = 0;

		long iTime = 0;

		// Define if this PP has Fading in and out effects
		bool bFadingProp = false;

		// Define the fade effect ( 0.0 - 1.0 ) 
		float fFadeEffect = 0.0f;

		// if fade effect has to be applied
		bool bToFade = false;

		// if fade effect require  this decide if fade in or out
		bool bFadeIn = false;

		// if is actually active
		bool bActive = false;;

		// Store Shader pointer
		Leadwerks::Shader *pShader = nullptr;

		bool bIsOK = false;
		
	public:
		// Construct class and load shader
		cPostProcess( std::string FilePath, float StartFadeEffect = 0.0 );
		cPostProcess( Leadwerks::Shader *Shader );
		~cPostProcess();
		inline bool IsOK()						{ return bIsOK; }

		// Properties
		inline bool HasFadingProp()				{ return bFadingProp; }
		inline bool IsActive()					{ return bActive; }

		// Properties Set
		cPostProcess *SetActive( bool Active );
		cPostProcess *SetFadeTime( long Time );
		cPostProcess *SetVisibility( float Value );

		// Properties Get
		inline float GetVisibility()			{ return fFadeEffect; }

		// Relative properties functions
		inline void FadeIn()					{ if ( bFadingProp ) bFadeIn = true;  }
		inline void FadeOut()					{ if ( bFadingProp ) bFadeIn = false; }

		// Core shader
		inline Leadwerks::Shader *GetShader()	{ return pShader; }

		bool Update();
};