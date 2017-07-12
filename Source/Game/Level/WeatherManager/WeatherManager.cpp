

#include "WeatherManager.h"

#include "Game\Level\Level.h"
#include "Game\Level\World.h"
#include "Game\Camera\CamManager.h"
#include <Engine.h>

#include "Effects\Fog.h"
#include "Effects\Rain.h"
#include "Effects\Sun.h"


#define DAY_LENGTH 86400.f

void LevelUpdateHook ( void ) { Level()->WeatherManager()->Update(); }


// Constructor
cWeatherManager::cWeatherManager() {

	std::string sStartTime = "09:00:00";
	cSection *pSection = LTXReader.GetSection( "Time" );
	if ( pSection ) sStartTime = pSection->GetString( "StartTime", sStartTime );
	
	this->TansformTime( sStartTime, fDayTimeNow );

	bIsOK = true;
}


// Destructor
cWeatherManager::~cWeatherManager() {

	this->Reset();

	for ( auto &pDescriptor : vEnvDescriptors )
		SAFE_DELETE( pDescriptor );

	vEnvDescriptors.clear();
	SAFE_RELEASE( pGlobalLight );

}


// Public
bool cWeatherManager::Initialize() {

	if ( !bIsOK ) return false; // leave bIsInitialized == false

	if ( bIsInitialized ) {
		cMsgCRT( "cWeatherManager::Initialize:Already initialized!!!" );
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Create Effects
	{
		{	// Create rain Emitter
			if ( !( pRainEffect = new cRainEffect() ) || !pRainEffect->IsOK() || !pRainEffect->Initialize() ) {
				cMsgCRT( "cWeatherManager::Load:Rain Effect class creation error" );
				return this->Reset();
			}
//			pRainEffect->SetFactor( 1.0f );
		}
		{	// Load fog shader
			if ( !( pFogEffect = new cFogEffect() ) || !pFogEffect->IsOK() ) {
				cMsgCRT( "cWeatherManager::Load:Fog Effect class creation error" );
				return this->Reset();
			}
		}
		{	// Create a spotlight with volumetric light effect
			if ( !( pSunEffect = new cSunEffect() ) || !pSunEffect->IsOK() ) {
				cMsgCRT( "cWeatherManager::Load:Sun Effects class creation error" );
				return this->Reset();
			}
			// Enable volumetric effect only with Sun Quality bigger than 1
			iSunQuality = Settings.GetSection( "Quality" )->GetInt( "SunQuality", 0 );
			pSunEffect->SetVolumetric( iSunQuality > 1 ); // so is enabled only for level 2 or 3
		}
	}

	Engine()->RegisterPeriodicHook( 1000 / 60, LevelUpdateHook ); // 15 times per socond, every 66.66666ms

	bIsInitialized = true;
	return true;
}


// Public
// This function should prepare for new level loading
bool cWeatherManager::Reset() {
	
	SAFE_DELETE( pEnvDescriptorMixer );

	SAFE_DELETE( pRainEffect );
	SAFE_DELETE( pFogEffect );
	SAFE_DELETE( pSunEffect );
	
	SAFE_RELEASE( pSkyMaterial );
	SAFE_RELEASE( pSkyShader );

	return false;

}

// Private
bool cWeatherManager::LoadDescriptors( std::string LevelWheater ) {

	////////////////////////////////////////////////////////////////////////////////////////
	// Find path of level weather
	std::string sCmpletePath = CONFIG_FILES_PATH + "\\Weathers\\" + LevelWheater + ".ltx";

	if ( !FileExists( sCmpletePath ) ) {
		cMsgCRT( "cWeatherManager::Load:Weather file doesn't exist :" + sCmpletePath );
		return this->Reset();
	}
	////////////////////////////////////////////////////////////////////////////////////////
	// Load descriptors
	cLTXReader *pLTX_Reader = new cLTXReader( sCmpletePath );
	{
		for ( const auto &pPair : pLTX_Reader->GetData() ) {

			// pair.first  is a string
			// piar.second is a cFileElement
			cSection *pSection = pPair.second->GetSection();
			std::string sTime = pPair.first; // "00:00:00"

			////////////////////////////////////////////////////////////////////////////////////////
			// Actual descriptor execution time
			float fExecTime = ZERO_FLOAT;

			////////////////////////////////////////////////////////////////////////////////////////
			// Parse string and set execution time
			if ( !this->TansformTime( sTime, fExecTime ) ) {
				SAFE_DELETE( pLTX_Reader );
				return this->Reset();
			}

//			cMsgDBG( "%s [ %s ] ExecTime = %f", LevelWheater.c_str(), sTime.c_str(), fExecTime );

			////////////////////////////////////////////////////////////////////////////////////////
			// Create descriptor, fill by section infos and load texture
			cEnvDescriptor *pEnvDescriptor = new cEnvDescriptor( sTime, fExecTime );
			if ( !pEnvDescriptor->Load( pSection ) ) {
				cMsgCRT( "cWeatherManager::Load:Cannot load descriptor %s", sTime.c_str() );
				return this->Reset();
			}

			pEnvDescriptor->LoadTexture();

			////////////////////////////////////////////////////////////////////////////////////////
			// Link one descriptos to previous one if exists
			if ( vEnvDescriptors.size() > 0 ) vEnvDescriptors.back()->pNext = pEnvDescriptor;

			////////////////////////////////////////////////////////////////////////////////////////
			// Insert the new descriptor into vector
			vEnvDescriptors.push_back( pEnvDescriptor );
		}
	}
	SAFE_DELETE( pLTX_Reader );
	
	return true;

}


// Public
bool cWeatherManager::Load( std::string LevelWheater, std::string SkyMixerShader ) {

	if ( !bIsInitialized ) return false;

	if ( bLoaded ) this->Reset();

	if ( fDayTimeNow == -1.0f ) {
		cMsgCRT( "cWeatherManager::Load:A day time must be set. Use SetTime first of this function." );
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Load Descriptors
	if ( !this->LoadDescriptors( LevelWheater ) ) return false;

	////////////////////////////////////////////////////////////////////////////////////////
	// This is the sun, mainly used for shadows
	{
		pGlobalLight = Leadwerks::DirectionalLight::Create();
		pGlobalLight->SetShadowMode( Leadwerks::Light::Static + Leadwerks::Light::Dynamic );
		pGlobalLight->SetIntensity( 1.0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Create material for sky
	{
		pSkyMaterial = Leadwerks::Material::Create();
		if ( !pSkyMaterial->SetShader( SkyMixerShader ) ) {
			cMsgCRT( "cWeatherManager::Load:Cannot load shader %s for sky texture", SkyMixerShader );
			return this->Reset();
		}
		// Retrieve sky material shader
		pSkyShader = pSkyMaterial->GetShader();
	}
	
	// WEATHER SETUP
	////////////////////////////////////////////////////////////////////////////////////////
	// Select start skies according to the selected day time
	this->SelectDescriptors( pEnvDescriptorCurrent, pEnvDescriptorNext, fDayTimeNow );

	////////////////////////////////////////////////////////////////////////////////////////
	// Create skies mixer
	pEnvDescriptorMixer = new cEnvDescriptorMixer( 0.0f );

	////////////////////////////////////////////////////////////////////////////////////////
	// Current thread continue running while textures are loaded
	// TODO : Choose a better way
	Leadwerks::Texture::SetLoadingMode( Leadwerks::Texture::DeferredLoadingMode );

	////////////////////////////////////////////////////////////////////////////////////////
	// If needed load sky textures and assing them to sky mixer shader
	this->UpdateSkies();

	////////////////////////////////////////////////////////////////////////////////////////
	// Interpolates ambient variables between actual descriptors
	this->EnvironmentLerp();

	////////////////////////////////////////////////////////////////////////////////////////
	// Set Camera Skybox
	CamManager()->GetCamera()->SetSkybox( pSkyMaterial );

	return true;
}

// Public
void cWeatherManager::SetSunShader( Leadwerks::Shader *Shader ) { pSunEffect->SetShader( Shader ); }

// Public
void cWeatherManager::SetFogShader( Leadwerks::Shader *Shader ) { pFogEffect->SetShader( Shader ); }

// Public
Leadwerks::Vec3 cWeatherManager::GetAmbientColor( void ) {

	if ( !bIsInitialized || !pGlobalLight ) return EMPTY_VEC3;

	return pEnvDescriptorMixer->vAmbientColor;

}

// Private
void cWeatherManager::UpdateSkies( void ) {

	////////////////////////////////////////////////////////////////////////////////////////
	// If needed load sky textures
	if ( !pEnvDescriptorCurrent->pSkyTexture )	pEnvDescriptorCurrent->LoadTexture();
	if ( !pEnvDescriptorNext->pSkyTexture )		pEnvDescriptorNext->LoadTexture();

	////////////////////////////////////////////////////////////////////////////////////////
	// Assing them to sky mixer shader
	pSkyMaterial->SetTexture( pEnvDescriptorCurrent->pSkyTexture, 0 );
	pSkyMaterial->SetTexture( pEnvDescriptorNext->pSkyTexture, 1 );

}


// Private
float cWeatherManager::TimeDiff( float Current, float Next ) {

    if ( Current > Next )
		return  ( (DAY_LENGTH - Current ) + Next );
    else {
		return ( Next - Current );
	}

}


// Private
float cWeatherManager::TimeInterpolant( float DayTime, float Current, float Next ) {

    float fInterpolant = 0.0f;
    float fLength = TimeDiff( Current, Next );
    if ( !SimilarZero( fLength, EPS ) ) {
        if (Current > Next)  {
            if ( ( DayTime >= Current ) || ( DayTime <= Next ) ) fInterpolant = TimeDiff( Current, DayTime ) / fLength;
        } else {
            if ( ( DayTime >= Current ) && ( DayTime <= Next ) ) fInterpolant = TimeDiff( Current, DayTime ) / fLength;
        }
        Leadwerks::Math::Clamp( fInterpolant, 0.0f, 1.0f );
    }
    return fInterpolant;

}


inline bool lb_env_pred( const cEnvDescriptor* x, float DayTime ) { return x->fExecTime < DayTime; }

// Private
void cWeatherManager::SelectDescriptors( cEnvDescriptor*& Current, cEnvDescriptor*& Next, const float DayTime ) {

	EnvIt env = std::lower_bound( vEnvDescriptors.begin(), vEnvDescriptors.end(), DayTime, lb_env_pred );
	if ( env == vEnvDescriptors.end() ) {
		Next = vEnvDescriptors.front();
		Current = vEnvDescriptors.back();
	} else {
		Next = *env;
		Current = ( env == vEnvDescriptors.begin() ) ? vEnvDescriptors.back() : *( env - 1 );
	}

}


// Private
void cWeatherManager::SelectDescriptors( const float DayTime ) {

	bool bSelect = false;
	if ( pEnvDescriptorCurrent->fExecTime > pEnvDescriptorNext->fExecTime ) {
		bSelect = ( DayTime > pEnvDescriptorNext->fExecTime ) && ( DayTime < pEnvDescriptorCurrent->fExecTime );
	} else {
		bSelect = ( DayTime > pEnvDescriptorNext->fExecTime );
	}

	if ( bSelect ) {
		pEnvDescriptorCurrent = pEnvDescriptorNext;
		pEnvDescriptorNext = ( pEnvDescriptorNext->pNext ) ?  pEnvDescriptorNext->pNext : vEnvDescriptors.front();
		
		this->UpdateSkies();
	}

}


// Private
void cWeatherManager::EnvironmentLerp( void ) {

	float fInterpolant = this->TimeInterpolant( this->fDayTimeNow, pEnvDescriptorCurrent->fExecTime, pEnvDescriptorNext->fExecTime );
	this->pSkyShader->SetFloat( "fPhaseFade", fInterpolant );
	this->pEnvDescriptorMixer->Lerp( pEnvDescriptorCurrent, pEnvDescriptorNext, fInterpolant );

}


// Public
void cWeatherManager::Update() {

	if ( !bIsInitialized || !pGlobalLight ) return;

	////////////////////////////////////////////////////////////////////////////////////////
	// if wheather is dynamic update DayTime
	if ( bDynamic ) {
		fDayTimeNow += Level()->GetTimeFactor();
		if ( fDayTimeNow > DAY_LENGTH ) fDayTimeNow = ZERO_FLOAT;

		SelectDescriptors( fDayTimeNow );
		EnvironmentLerp();
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Ambient Color
	Leadwerks::Vec3 vAmbCol( pEnvDescriptorMixer->vAmbientColor );
	World()->GetWorld()->SetAmbientLight( vAmbCol.r, vAmbCol.g, vAmbCol.b );

	////////////////////////////////////////////////////////////////////////////////////////
	//	Sky Color;
	pSkyShader->SetVec3( "vSkyColor", pEnvDescriptorMixer->vSkyColor );

	////////////////////////////////////////////////////////////////////////////////////////
	// Sun Rotation
	Leadwerks::Vec3 vSunRotation = pEnvDescriptorMixer->vSunRotation;

	////////////////////////////////////////////////////////////////////////////////////////
	// Sun ( Directional Light )
	pGlobalLight->AlignToVector( vSunRotation );
	pGlobalLight->SetColor( Leadwerks::Vec4( pEnvDescriptorMixer->vSunColor, 1.0 ) );

	////////////////////////////////////////////////////////////////////////////////////////
	// Update Rain effect
//	pRainEffect->Update();

	////////////////////////////////////////////////////////////////////////////////////////
	// Update Fog Effect
	pFogEffect->SetFactor( pEnvDescriptorMixer->fFogFactor );
	pFogEffect->Update();

	////////////////////////////////////////////////////////////////////////////////////////
	// Update SunEffects ( Godrays + SunGlow + Volumetric light )
	pSunEffect->SetColor( pEnvDescriptorMixer->vSunColor );
	pSunEffect->SetGradientRadius( pEnvDescriptorMixer->fGradientRadius );
	pSunEffect->AdjustQuality( iSunQuality );
	pSunEffect->Update( vSunRotation );

}