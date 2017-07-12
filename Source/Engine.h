
#pragma once

#include <algorithm>    // std::for_each

#include "Defines.h"

#include "Leadwerks.h"
#include "Graphics\Renderer.h"
#include "UserInterface\UI_Manager.h"
#include "Audio\Audio.h"
#include "Utils\Utils.h"
#include "Utils\LTXReader\LTXReader.hpp"
#include "ScriptEngine\ScriptEngine.h"
#include "InputManager.h"

namespace EngineStates {

	
		const int LAUNCH			= 0;
		const int MAINMENU			= 1;
		const int LOADING_LEVEL		= 2;
		const int INGAME			= 4;
		const int ENDGAME			= 5;

		const int PAUSE				= 6;

};

class cStateClass {
	private:
		unsigned int uState	= ZERO_INT;
	public:
		inline void	OnlyState( int S )			{ uState = S; };
		inline void	SetState( int S, bool b )	{ b ? AddState( S ) : RemState( S ); }
		inline bool	HasState( int S )			{ return ( uState&S ) ? true:false; };	
		inline void	AddState( int S )			{ if ( !HasState( S ) ) uState |=  S; };
		inline void	RemState( int S )			{ if ( HasState( S ) )	uState &= ~S; };
		inline void	ToogleState( int S )		{ uState ^= S; };
		inline void ClearState()				{ uState = ZERO_INT; }
};

class cEntitySection {
	private:
		cSection *pSection	= nullptr;
		bool bAssigned		= false;

	public:
		inline void SetSection( cSection *Section ) {
			if ( Section ) {
				pSection = Section;
				bAssigned = true;
			}
		}
		inline bool		IsAssigned() { return bAssigned; }
		inline cSection *GetSection() { return pSection; }

};

class cCallbackOnce {
	private:
		bool bCallbackCalled = false;

	public:
		inline void ResetCallback() { bCallbackCalled = false; }
		inline void OnCallbackCall() { bCallbackCalled = true; }
		inline bool CallbackHasCalled() { return bCallbackCalled == true; }

};


typedef void( *HookFunction ) ( void );
typedef bool( *HookCondition ) ( void );
typedef bool( *PickCallback )( Leadwerks::PickInfo );


struct cPeriodicFunction {
	private:
		unsigned long int iID		= ZERO_INT;
		long int iExecuteTime		= ZERO_INT;
		long int iTimer				= ZERO_INT;
		HookFunction pFunction		= NULL;
		HookCondition pCondition	= NULL;
		bool bIsOK					= false;
	public:
		cPeriodicFunction( long int Time, HookFunction fn, HookCondition cond = NULL );
		inline unsigned long int	GetID() { return iID; }
		inline void					Update();
};


class cEngine {
	private:
		cRenderer *		pRenderer			= nullptr;
		cAudioDevice *	pAudioDevice		= nullptr;
		cScriptEngine *	pScriptEngine		= nullptr;
		cUI_Manager *	pUI_Manager			= nullptr;

		std::list < std::string >			vInfoQueue;

		unsigned long int iID				= ZERO_INT;
		std::vector< cPeriodicFunction * >	vPeriodicHooks;

		cInputManager *pInputManager		= NULL;

		long int	iUpdateTime				= ZERO_INT;

		int			iInternalState			= 0;

		bool		bNeedToQuit				= false;
		bool		bIsOK					= false;
		bool		bIsInitialized			= false;

	public:
		cEngine();
		~cEngine();
				bool			Initialize();
		inline	bool			IsOK()								{ return bIsOK; }
		inline	bool			IsInitialized()						{ return bIsInitialized; };
				bool			Reset();
		inline	void			QuitRequest()						{ bNeedToQuit = true; }

		inline	int				GetInternalState()					{ return iInternalState; }
		inline	void			SetInternalState( int i )			{ iInternalState = i; }

		inline	cRenderer *		Renderer()							{ return pRenderer; }
		inline	cAudioDevice *	AudioDevice()						{ return pAudioDevice; }
		inline	cScriptEngine *	ScriptEngine()						{ return pScriptEngine; }
		inline	cInputManager *	InputManager()						{ return pInputManager; }
		inline	cUI_Manager *	UI_Manager()						{ return pUI_Manager; }

	private:
		inline	void			DrawInfos();
				void			PrepareSettingsFile();

	public:
		inline	unsigned long 	NewID()								{ iID++; return iID; }
		inline	unsigned long 	RegisterPeriodicHook( long int Time, HookFunction fn, HookCondition cond = NULL );
		inline	void			UnRegisterPeriodicHook( unsigned long int ID );
	private:
		inline	void			CallPeriodicHooks();
	public:
				bool			StartWorld();

				void			DrawInfo( char *format, ... );
		inline	void			DrawInfo( std::string sInfo ) { DrawInfo( "%s", sInfo.c_str() ); }

		bool					Loop();
	
};

extern cEngine *pEngine;
inline cEngine *Engine() { if ( !pEngine ) pEngine = new cEngine(); return ( pEngine ); }

#include "Engine_inline.h"