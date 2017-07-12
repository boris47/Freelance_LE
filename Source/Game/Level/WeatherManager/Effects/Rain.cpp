
#include "Engine.h"

#include "Rain.h"
#include "Thunderbolt.h"
#include "Game\Level\World.h"
#include "Game\Camera\CamManager.h"
#include "..\WeatherManager.h"

#define RAIN_MAX_PARTICLES 400
#define RAIN_PARTICLES_DURATION 4000
#define RAIN_MAX_HEIGTH 40.f
#define RAIN_SPEED 20.0f

#define AREA_RANGE_WIDTH	10.0f
#define AREA_RANGE_HEIGTH	10.0f

#define POSITION_HEIGHT		30.0f

class cParticleData {
	public:
		float fFallHeight = ZERO_FLOAT;
		Leadwerks::Vec3 vSpawnPos = EMPTY_VEC3;
};

cRainEffect::cRainEffect() {

	// Rain pomplex emitter
	pRainEmitter = Leadwerks::Emitter::Create();
	pRainEmitter->SetEmissionVolume( AREA_RANGE_WIDTH, 0, AREA_RANGE_HEIGTH );
	pRainEmitter->SetDuration( RAIN_PARTICLES_DURATION );
	pRainEmitter->SetVelocity( 0.0, -RAIN_SPEED, 0.0 );
	pRainEmitter->SetLoopMode( true, true );
	pRainEmitter->SetViewMode( 0 ); // Rotate around Y axis(billboard)
	pRainEmitter->SetMaxScale( 0.3, 3.0 );
	pRainEmitter->SetParticleColor( 1.0, 1.0, 1.0, 0.6, 0 ); // 0 means start color, in my case the only color
	pRainEmitter->Show();

	bIsOK = true;

}

cRainEffect::~cRainEffect() {
	
	

}

void cRainEffect::Reset( void ) {

	// clear particles data
	for ( auto &Particle : pRainEmitter->particle ) {
		cParticleData *pInfo = ( cParticleData * ) Particle.GetUserData();
		SAFE_DELETE( pInfo );
	}
	pRainEmitter->particle.clear();
	SAFE_RELEASE( pRainEmitter );

	// clear thunderbolts instances
	for ( auto &pThunderboolt : vThunderbolts ) {
		SAFE_DELETE( pThunderboolt );
	}

}

bool cRainEffect::Initialize() {

	// load thunderbolts
	std::string sCmpletePath = CONFIG_FILES_PATH + "\\Weathers\\Thunderbolts.ltx";
	if ( !FileExists( sCmpletePath ) ) {
		cMsgCRT( "cRainEffect::Initialize:Thunderbolts file doesn't exist :" + sCmpletePath );
		return false;
	}

	cAudioDevice * pAudiodevice		= Engine()->AudioDevice();
	cLTXReader *pLTX_Reader			= new cLTXReader( sCmpletePath );
	for ( const auto &pPair : pLTX_Reader->GetData() ) {

		/*
			pPair.first = Section name
			pPair.seconf = Section Element { Delay = int, Duration = int }
		*/

//		Wheather.Thunders.thunder1
		std::string sSoundPath = String_Transform( "Wheather\\Thunders\\" + pPair.first );

		cAudioSrc *pAudioSrc = NULL;
		if ( !( pAudioSrc = pAudiodevice->FindSource( sSoundPath ) ) ) {
			cMsgCRT( "cRainEffect::Initialize:Cannot find sound \"%s\"", ( sSoundPath ).c_str() );
			this->Reset();
			return false;
		}

		cSection *pSection = pPair.second->GetSection();

		int iDelay		= pSection->GetInt( "Delay", ZERO_INT );
		float fDuration = pSection->GetFloat( "Duration", ZERO_FLOAT );
		cThunderbolt *pThunderBolt = new cThunderbolt( pAudioSrc, iDelay, fDuration );
		vThunderbolts.push_back( pThunderBolt );

	}


	SAFE_DELETE( pLTX_Reader );

	bIsInitialized = true;
	return true;

}

void cRainEffect::Resize( float Width, float Heigth ) {

	pRainEmitter->SetEmissionVolume( Width, 0, Heigth );

}

void cRainEffect::ParticleReposition( Leadwerks::Particle &Particle ) {
	
	Particle.Reset();
	Particle.SetSpawnPosition();
	Particle.Update();
	ParticleRayPick( Particle );

}

void cRainEffect::ParticleRayPick( Leadwerks::Particle &Particle ) {

	// Calculate collision distance
	Leadwerks::Vec3 vParticlePosition( Particle.position.x, Particle.position.y, Particle.position.z );
	Leadwerks::Vec3 vFinalPosition = vParticlePosition - Leadwerks::Vec3( 0.0, RAIN_MAX_HEIGTH, 0.0 );
	Leadwerks::PickInfo pInfo;
	bool bResult = World()->GetWorld()->Pick( vParticlePosition, vFinalPosition, pInfo );

	// Save inside internal vars
	cParticleData *pParticleInfo = ( cParticleData * ) Particle.GetUserData();
	pParticleInfo->fFallHeight = ( bResult ) ? ( vParticlePosition.DistanceToPoint( pInfo.position ) ): 0.0f;
	pParticleInfo->vSpawnPos = Particle.position;

}

void cRainEffect::UpdateHeights() {

	for ( auto &Particle : pRainEmitter->particle ) {
		cParticleData *pInfo = ( cParticleData * ) Particle.GetUserData();
		if ( !pInfo ) {
			Particle.SetUserData( new cParticleData );
//			Particle.randrotation = Particle.currentRotation = 0.0f;
		}
	}

}

void cRainEffect::Update() {

	if ( !IsActive() ) {
		pRainEmitter->Pause();
		return;
	} else {
		pRainEmitter->Play();
	}

	// update emitter position
	Leadwerks::Vec3 vPosition = CamManager()->GetCamera()->GetPosition( true );
	pRainEmitter->SetPosition( vPosition.x, vPosition.y + POSITION_HEIGHT, vPosition.z, true );

	// Rain factor controls particles count and velocity
	int iCount = RAIN_MAX_PARTICLES * fFactor;

	pRainEmitter->velocity[ 0 ].y = -( RAIN_SPEED + ( RAIN_SPEED * fFactor ) );

	// adjust particles count
	if ( iCount != pRainEmitter->GetParticleCount() ) {
		// manual deletion
		if ( iCount < pRainEmitter->GetParticleCount() ) { // must remove some particles
			while ( pRainEmitter->particlecount > iCount ) {
				cParticleData *pInfo = ( cParticleData * ) pRainEmitter->particle.back().GetUserData();
				SAFE_DELETE( pInfo );
				pRainEmitter->particle.pop_back(); // effectively remove last element
				pRainEmitter->particlecount--;
			}
		}
		else {
			pRainEmitter->SetParticleCount( iCount );
			pRainEmitter->Update(); // should effectively create the new partcles
		}
		UpdateHeights();
	}

	auto ProcessParticle = [ & ]( Leadwerks::Particle &Particle ) {
		Particle.randrotation = Particle.currentRotation = 0.0f;

		cParticleData *pInfo = ( cParticleData * ) Particle.GetUserData();
		if ( pInfo->fFallHeight > 0.0f ) {
			if ( Particle.position.DistanceToPoint( pInfo->vSpawnPos ) > pInfo->fFallHeight ) {
				ParticleReposition( Particle );
			}
		}
		// else is to parse
		else ParticleRayPick( Particle );
	};

	std::for_each( pRainEmitter->particle.begin(), pRainEmitter->particle.end(), ProcessParticle );

}