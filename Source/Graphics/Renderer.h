
#pragma once

#include "Leadwerks.h"

class cRenderer {
//	Leadwerks::GraphicsDriver*	pGraphicDriver;
	Leadwerks::Window*			pWindow				= NULL;
	Leadwerks::Context*			pContext			= NULL;

	int							iWidth, iHeight;
	int							iWinFlags;
	bool						bFullscreen, bVsync;

	bool						bIsOK				= false;
	bool						bIsInitialized		= false;

	public:
		cRenderer();
		~cRenderer();
				bool Initialize();
		inline	bool IsOK()								{ return bIsOK; }
		inline	bool IsInitialized()					{ return bIsInitialized; }

		inline	Leadwerks::Window *		GetWindow()		{ return pWindow; }
		inline	Leadwerks::Context *	GetContext()	{ return pContext; }

				void					DrawStats();
				void					DrawInfo( std::string Info, int x, int y );
				void					Update();
				void					Clear();
};