

#pragma once

#include <Engine.h>

class cEffectBase {

	protected:
		float fSpeedMul		= ONE_FLOAT;
		float fAmplitudeMul = ONE_FLOAT;

		bool bActive		= true;

	public:
		inline void				Enable()					{ bActive = true; }
		inline void				Disable()					{ bActive = false; }
		inline bool				IsActive()					{ return bActive; }

		inline void				SetSpeedMul( float Val )	{ fSpeedMul = Val; }
		inline float			GetSpeedMul()				{ return fSpeedMul; }

		inline void				SetAmplitudeMul( float Val ){ fAmplitudeMul = Val; }
		inline float			GetAmplitudeMul()			{ return fAmplitudeMul; }

};