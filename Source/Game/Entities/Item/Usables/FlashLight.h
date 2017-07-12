

#pragma once

#include <Engine.h>
#include "Usable.h"

class cFlashLight : public cItemUsable {
	private:
		Leadwerks::Vec3 vPosition		= EMPTY_VEC3;
		
		Leadwerks::Vec3 vRelPos			= EMPTY_VEC3;

		Leadwerks::Vec3 vDirection		= EMPTY_VEC3;

		Leadwerks::Vec3 vRelDir			= EMPTY_VEC3;

		bool bEnabled					= true;
		bool bActive					= false;

	public:
		
		cFlashLight();
		virtual ~cFlashLight();

		inline	Leadwerks::Vec3	GetPosition()						{ return vPosition; }

		// Set position relative to parent position
		inline	void			SetPosition( Leadwerks::Vec3 v )	{ vRelPos = v; }

		inline	Leadwerks::Vec3	GetRotation()						{ return vDirection; }

		inline	void			SetRotation( Leadwerks::Vec3 v )	{ vRelDir = v; }

		inline	void			Enable()							{ bEnabled = true; }
		inline	void			Disable()							{ bEnabled = false; }
		inline	bool			IsEnabled()							{ return bEnabled; }

		inline	void			Activate()							{ bActive = true;  pEntity->Show(); };
		inline	void			Deactivate()						{ bActive = false; pEntity->Hide(); };
		inline	bool			IsActive()							{ return bActive; }
		
				void			Update();
				void			OnFrame() {}
};