
#pragma once


// Foreward declaration
class cThunderbolt;


class cRainEffect {

	private:

		Leadwerks::Emitter *	pRainEmitter	= NULL;

		std::vector < cThunderbolt * >			vThunderbolts;
		
		float					fFactor			= ZERO_FLOAT;
		
		bool					bIsOK			= false;
		bool					bIsInitialized	= false;

	public:
		cRainEffect();
		~cRainEffect();
				bool Initialize();
		inline	bool IsOK( void )						{ return bIsOK; }
		inline	bool IsInitialized( void )				{ return bIsInitialized; }

				void Reset( void );

		inline	void SetFactor( float F )				{ fFactor = L_CLAMP( F, 0.0f, 1.0f ); }

		inline	bool IsActive( void )					{ return fFactor > 0.0f; }

				void Resize( float Width, float Heigth );

	private:
				void ParticleReposition( Leadwerks::Particle &Particle );
				void ParticleRayPick( Leadwerks::Particle &Particle );
				void UpdateHeights();

	public:

				void Update( void );

};