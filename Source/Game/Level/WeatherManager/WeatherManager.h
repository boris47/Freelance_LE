
#pragma once

#include <Engine.h>
#include "EnvDescriptor.h"
#include "Effects\Fog.h"
#include "Effects\Rain.h"
#include "Effects\Sun.h"
#include "Effects\Thunderbolt.h"

#define VALID_TIME( h, m, s ) ( ( h >= 0 ) && ( h < 24 ) && ( m >= 0 ) && ( m < 60 ) && ( s >= 0 ) && ( s < 60 ) )

class cWeatherManager {
	private:

		std::vector< cEnvDescriptor* >						vEnvDescriptors;
		typedef std::vector< cEnvDescriptor* >::iterator	EnvIt;

		// Descriptors
		cEnvDescriptor *pEnvDescriptorCurrent				= NULL;
		cEnvDescriptor *pEnvDescriptorNext					= NULL;
		cEnvDescriptorMixer *pEnvDescriptorMixer			= NULL;


		// Global light
		Leadwerks::DirectionalLight *pGlobalLight			= NULL;
		

		// Sky
		float fFadingFactor									= ZERO_FLOAT;
		Leadwerks::Material *pSkyMaterial					= NULL;
		Leadwerks::Shader *pSkyShader						= NULL;


		// Environment
		cRainEffect * pRainEffect							= NULL;
		cFogEffect * pFogEffect								= NULL;
		cSunEffect * pSunEffect								= NULL;


		unsigned char iSunQuality							= ZERO_INT;

		float fDayTimeNow									= -1.0f;
		bool bDynamic										= false;

		bool bLoaded										= false;
		bool bIsOK											= false;
		bool bIsInitialized									= false;

	public:
		cWeatherManager();
		~cWeatherManager();
				bool 							Initialize();
		inline	bool 							IsOK( void )					{ return bIsOK; }
		inline	bool 							IsInitialized( void )			{ return bIsInitialized; }
		inline	bool 							IsLoaded( void )				{ return bLoaded; }
				bool 							Reset();

	private:
				bool							LoadDescriptors( std::string LevelWheater );

	public:
				bool 							Load( std::string LevelWheater, std::string SkyMixerShader );
		// Properties
		inline	void							SetDynamic( bool Val )			{ bDynamic = Val; }
		inline	void							SeSunQuality( int i )			{ iSunQuality = L_CLAMP( i, 0, 3 ); }
		inline	int								GeSunQuality( void )			{ return iSunQuality; }

		// Time
		inline	bool							TansformTime( std::string sTime, float &Time );
		inline	void							SetTime( float DayTime )		{ fDayTimeNow = DayTime;	}
		inline	void							SetTime( int H, int M, int S )	{ fDayTimeNow = float( ( H * 3600 ) + ( M * 60 ) + S ); }
		inline	void							SetTime( std::string sTime )	{ TansformTime( sTime, fDayTimeNow ); }
		inline	float							GetTime( void )					{ return fDayTimeNow; }
		inline	std::string						GetTimeAsString( float f );
		inline	std::string						GetActualTimeAsString( void )	{ return GetTimeAsString( fDayTimeNow ); }

		// Objects and effects
		inline	Leadwerks::DirectionalLight *	GetSun( void )					{ return pGlobalLight; }
		inline	cRainEffect *					GetRainEffect( void )			{ return pRainEffect; }
		inline	cFogEffect *					GetFogEffect( void )			{ return pFogEffect; }
		inline	cSunEffect *					GetSunEffect( void )			{ return pSunEffect; }

				void							SetSunShader( Leadwerks::Shader *Shader );
				void							SetFogShader( Leadwerks::Shader *Shader );

				Leadwerks::Vec3					GetAmbientColor( void );

	private:
				float							TimeDiff( float prev, float cur);
				float 							TimeInterpolant( float val, float min_t, float max_t );

				void 							EnvironmentLerp( void );

				void 							UpdateSkies( void );
				void 							SelectDescriptors( cEnvDescriptor*& Current, cEnvDescriptor*& next, const float DayTime );
				void 							SelectDescriptors( const float DayTime );

		
	public:
				void Update();
	
};

// Public
bool cWeatherManager::TansformTime( std::string sTime, float &Time ) {
	int iH = ZERO_INT, iM = ZERO_INT, iS = ZERO_INT;
	if ( ( sscanf( sTime.c_str(), "%d:%d:%d", &iH, &iM, &iS ) < 1 ) || !VALID_TIME( iH, iM, iS ) ) {
		cMsgCRT( "cWeatherManager::TansformTime:Incorrect weather time, %s", sTime.c_str() );
		return false;
	}
	Time = float( ( iH * 3600.f ) + ( iM * 60.f ) + iS );
	return true;
}

// Public
std::string cWeatherManager::GetTimeAsString( float f ) {

	int iH = ( int ) ( f / ( 60 * 60 ) );
	int iM = ( int ) ( f / 60 ) % 60;
	int iS = ( int ) f % 60;

	char buffer[ 255 ];
	sprintf( buffer, "%02d:%02d:%02d", iH, iM, iS );

//	std::ostringstream out;
//	out << ( int ) ( f / ( 60 * 60 ) ) << ":" << ( int ) ( f / 60 ) % 60 << ":" << ( int ) f % 60;
//	return out.str();

	return std::string( buffer );
}