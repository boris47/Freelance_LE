

#pragma once

#include "Engine.h"

/*
la luce che ad intermittenza si accende
e il suono che con ritardo viene riprodotto
*/

class cThunderbolt {

	private:
		int iDelay						= ZERO_INT;		// salva il tempo al quale eseguire il suono
		Leadwerks::Vec3 vPosition		= EMPTY_VEC3;
		Leadwerks::Vec3 vRotation		= EMPTY_VEC3;

		float fFlashTime				= ZERO_FLOAT;	// conserva l'intervallo fra un flash e l'altro
		float fDuration					= ZERO_FLOAT;	// durata sparaflesciamento
		cAudioSrc *	pAudioSrc			= NULL;			// suono da riprodurre

		bool bIsActive = false;

	public:

		cThunderbolt( cAudioSrc * Source, int Delay, float Duration );

		inline	void Enable()							{ bIsActive = true; }
		inline	void Disable()							{ bIsActive = false; }
		inline	bool IsEnabled()						{ return bIsActive; }

		inline	void SetPosition( Leadwerks::Vec3 p )	{ vPosition = p; }
		inline	void SetRotation( Leadwerks::Vec3 r )	{ vRotation = r; }

		void Update();

		inline	void Reset() {
			vPosition = vRotation = EMPTY_VEC3;
			fFlashTime = fDuration = ZERO_FLOAT;
			bIsActive = false;
		}

};