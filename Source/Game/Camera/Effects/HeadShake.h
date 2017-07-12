
#pragma once

#include <Engine.h>

class cHeadShake {
	private:
		
		Leadwerks::Vec3 vDirection = EMPTY_VEC3;

		Leadwerks::Vec3 vTargetDir = EMPTY_VEC3;


		float fAmplitude = ZERO_FLOAT;
		float fXSpeed = ZERO_FLOAT;
		float fYSpeed = ZERO_FLOAT;
		long iDuration = ZERO_INT;
		long iTime = ZERO_INT;

		float fXTheta = ZERO_FLOAT;
		float fYTheta = ZERO_FLOAT;
		float fOldCos = ZERO_FLOAT;

		bool bActive = false;

	public:
		void Shake( float fAmplitudeMul = 1.0f, float fSpeedMul = 1.0f, long iDuration = 0 );

		Leadwerks::Vec3 GetDirection() { return vDirection; }

		void Enable( void ) { bActive = true; }
		void Disable ( void ) { bActive = false; }
		bool IsActive( void ) { return bActive; }

		void SetAmplitude( float fVal )	{ fAmplitude= abs( fVal ); }
		float GetAmplitude( void )		{ return fAmplitude; }

		void SetSpeed( float fVal )		{ fXSpeed = fYSpeed	= abs( fVal ); }
		float GetSpeed( void )			{ return fXSpeed; }

		void SetDuration( long iVal )	{ iDuration = abs( iVal ); }
		long GetDuration( void )		{ return iDuration; }

		void Update( void );

		void OnFrame( void );

		void Reset( bool bInstant );

};