
#pragma once

#include <Engine.h>
#include "PostProcess.h"

class cPostProcessManager {
	private:
		// Is the camera pointer
		Leadwerks::Camera *pCamera = NULL;
		bool bIsOK = false;

		// store loaded shaders
		std::map < std::string, cPostProcess *> mLoaded;

	public:
		// Construct PostProcess Manager
		cPostProcessManager( Leadwerks::Camera *Camera );
		~cPostProcessManager();
		inline bool		IsOK() { return bIsOK; }

		cPostProcess	*IsLoaded( std::string Name );

		cPostProcess	*LoadLua ( std::string FilePath, std::string Name );

		// Load and strore Shader loaded
		cPostProcess	*Load( std::string FilePath, std::string Name, float StartFadeEffect = 0.0 );

		// Returns if specific PP is actually active
		bool			IsActive( std::string Name );

		// Apply fading IN the shader
		bool			FadeIn( std::string Name, unsigned int Time = 1 );

		// Apply fading OUT the shader
		bool			FadeOut( std::string Name, unsigned int Time = 1 );

		// Directly apply shader
		bool			Apply( std::string Name );

		// Directly Remove shader
		bool			Remove( std::string Name );

		// Clear every post effect
		void			Clear( void );

		// Update all times and shaders
		void			Update( void );

};