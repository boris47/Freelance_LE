
#pragma once

#include <Engine.h>

class cZoom {
	private:
		// Zoom
		float fCamZoom			= ZERO_FLOAT;

		float fCamZoomActual	= ZERO_FLOAT;
		float fCamZoomOrig		= ZERO_FLOAT;

		float fCamViewDist		= ZERO_FLOAT;

		bool bZoomed			= false;
		bool bActive			= true;

	public:
		// Constructor
		cZoom( Leadwerks::Camera *pCamera );
		
		inline float	GetCamZoom()	{ return fCamZoom; }
		inline bool		IsZoomed()		{ return bZoomed; };

		inline void		Enable()		{ bActive = true; }
		inline void		Disable()		{ bActive = false; Reset(); }
		inline bool		IsActive()		{ return bActive; }

		void Update();

		void Reset();

};