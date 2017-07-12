
#pragma once


class cFogEffect {

	private:
		Leadwerks::Shader *pShader		= NULL;

		float fFactor					= ZERO_FLOAT;
		Leadwerks::Vec3 vColorMult		= Leadwerks::Vec3( 1.0f );

		bool bIsOK						= false;
		bool bIsInitialized				= false;

	public:
		cFogEffect();
		~cFogEffect();

				bool	Initialize();
		inline	bool	IsOK( void )						{ return bIsOK; }
		inline	bool	IsInitialized( void )				{ return bIsInitialized; }

		inline	void	SetShader( Leadwerks::Shader *p )	{ pShader = p; }

		inline	void	SetFactor( float F )				{ fFactor = L_CLAMP( F, 0.0f, 1.0f ); }
		inline	float	GetFactor( void )					{ return fFactor; }

		inline void		SetColorMulti( Leadwerks::Vec3 c )	{ vColorMult = c; }

		inline	bool	IsActive( void )					{ return fFactor > 0.0f; }

				void	Update( void );
};