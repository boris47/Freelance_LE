
#pragma once

class cSunEffect {

	private:
		Leadwerks::SpotLight *	pLight			= NULL;
		Leadwerks::Sprite *		pMoonSprite		= NULL;
		Leadwerks::Vec3			vSunColor		= EMPTY_VEC3;

		Leadwerks::Shader *pShader				= NULL;

		int		iQuality						= ZERO_INT;
		float	fGradientRadius					= ZERO_FLOAT;
		bool	bVolumetric						= false;

		bool	bIsOK							= false;
		bool	bIsInitialized					= false;

	public:
		cSunEffect();
		~cSunEffect();

				bool Initialize();
		inline	bool IsOK( void )						{ return bIsOK; }
		inline	bool IsInitialized( void )				{ return bIsInitialized; }

		inline	void SetShader( Leadwerks::Shader *p )	{ pShader = p; }

		inline	void SetColor( Leadwerks::Vec3 c )		{ vSunColor = c; }

		inline	void SetVolumetric( bool b )			{ b ? pLight->Show() : pLight->Hide(); }

		inline	void AdjustQuality( int i )				{ iQuality = i; }

		inline	void SetGradientRadius( float f )		{ fGradientRadius = f; }

				void Update( Leadwerks::Vec3 vSunRotation );

};