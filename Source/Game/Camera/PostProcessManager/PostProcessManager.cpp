
#include "PostProcessManager.h"

// Costructor
cPostProcessManager::cPostProcessManager( Leadwerks::Camera *Camera ) {

	if ( !Camera ) return; // leave bIsOK false
	
	pCamera = Camera;

	// prepare containers
	mLoaded.clear();

	bIsOK = true;

}


// Destructor
cPostProcessManager::~cPostProcessManager() {

	pCamera->posteffects.clear();
	
	// free all post processes
	for ( auto &pPair : mLoaded ) {
		cPostProcess *pPostProcess = pPair.second;
		delete pPostProcess; pPostProcess = NULL;
	}

	mLoaded.clear();

}


// Public
cPostProcess *cPostProcessManager::IsLoaded( std::string Name ) {
	
	if ( !bIsOK ) return NULL;

	cPostProcess *pPostProcess = NULL;
	try { pPostProcess = mLoaded.at( Name ); }
	catch ( ... ) { return NULL; }

	return pPostProcess;
}


// Public
cPostProcess *cPostProcessManager::LoadLua ( std::string FilePath, std::string Name ) {
	
	if ( !bIsOK ) return NULL;

	if ( !FileExists( FilePath ) ) {
		cMsgCRT( "cPPManager::LoadLua:File %s cannot be found", FilePath.c_str() );
		return NULL;
	}

	if ( GetExtFromFilePath( FilePath ) != ".lua" ) {
		cMsgCRT( "cPPManager::LoadLua:Trying to load %s, for .shader files use Load", FilePath.c_str() );
		return NULL;
	}

	if ( IsLoaded( Name ) ) {
		cMsgCRT( "cPPManager::LoadLua:Already loaded post process %s", FilePath.c_str() );
		return NULL;
	}

	int ID = pCamera->AddPostEffect( FilePath );
	if ( ID == -1 ) {
		cMsgCRT( "cPPManager::LoadLua::Cannot add post porcess %s", FilePath.c_str() );
		return NULL;
	}

	cPostProcess *pPostProcess = new cPostProcess( (Leadwerks::Shader*)pCamera->posteffects[ ID ] );
	if ( !pPostProcess->IsOK() ) {
		delete pPostProcess;
		return NULL; // Error manager by cPP class
	}

	// Insert in camera effects and applied vector
	mLoaded[ Name ] = pPostProcess;

	cMsgDBG( "cPPManager::LoadLua:Loaded shader %s", FilePath.c_str() );

	return pPostProcess;
}


// Public
cPostProcess *cPostProcessManager::Load( std::string FilePath, std::string Name, float StartFadeEffect ) {

	if ( !bIsOK ) return NULL;

	if ( !FileExists( FilePath ) ) {
		cMsgCRT( "cPPManager::Load:File %s cannot be found", FilePath.c_str() );
		return NULL;
	}

	if ( GetExtFromFilePath( FilePath ) != ".shader" ) {
		cMsgCRT( "cPPManager::Load:Trying to load %s, for .lua files use LoadLua", FilePath.c_str() );
		return NULL;
	}

	if ( IsLoaded( Name ) ) {
		cMsgCRT( "cPPManager::Load:Already loaded post process %s", FilePath.c_str() );
		return NULL;
	}

	// Create INACTIVE instance of PP
	cPostProcess *pPostProcess = new cPostProcess( FilePath, StartFadeEffect ); // by Default StartFadeEffect = 0.0, so is hidden
	if ( !pPostProcess->IsOK() ) {
		delete pPostProcess;
		return NULL; // Error manager by cPostProcess class
	}

	// Insert in camera effects and applied vector
	pCamera->posteffects.push_back( pPostProcess->GetShader() );
	mLoaded[ Name ] = pPostProcess;

	cMsgDBG( "cPPManager::Load:Loaded shader %s", FilePath.c_str() );

	return pPostProcess;
}


// Public
bool cPostProcessManager::IsActive( std::string Name ) {

	if ( !bIsOK ) return false;

	cPostProcess *pPostProcess = IsLoaded( Name );
	if ( !pPostProcess ) {
		cMsgCRT( "cPPManager::IsActive:Cannot find post process %s", Name.c_str() );
		return false;
	}

	return pPostProcess->IsActive();
}


// Public
bool cPostProcessManager::FadeIn( std::string Name, unsigned int Time ) {

	if ( !bIsOK ) return false;

	cPostProcess *pPostProcess = IsLoaded( Name );
	if ( !pPostProcess ) {
		cMsgCRT( "cPPManager::FadeIn:Cannot find post process %s", Name.c_str() );
		return false;
	}

	if ( !pPostProcess->HasFadingProp() ) {
		cMsgCRT( "cPPManager::FadeIn:Post process %s has no fading property", Name.c_str() );
		return false;
	}

	// if already applied then return false
	if ( IsActive( Name ) ) return false;

	cMsgDBG( "cPPManager::FadeIn:\"%s\"", Name.c_str() );

	pPostProcess->SetFadeTime( Time );
	pPostProcess->FadeIn();

	// Set as active
	pPostProcess->SetActive( true );

	return true;
}


// Public
bool cPostProcessManager::FadeOut( std::string Name, unsigned int Time ) {

	if ( !bIsOK ) return false;

	cPostProcess *pPostProcess = IsLoaded( Name );
	if ( !pPostProcess ) {
		cMsgCRT( "cPPManager::FadeOut:Cannot find post process %s", Name.c_str() );
		return false;
	}

	if ( !pPostProcess->HasFadingProp() ) {
		cMsgCRT( "cPPManager::FadeIn:Post process %s has no fading property", Name.c_str() );
		return false;
	}

	// if not already applied then return false
	if ( !IsActive( Name ) ) return false;

	cMsgDBG( "cPPManager::FadeOut:\"%s\"", Name.c_str() );

	pPostProcess->SetFadeTime( Time );
	pPostProcess->FadeOut();

	return true;
}


// Public
bool cPostProcessManager::Apply( std::string Name ) {

	if ( !bIsOK ) return false;

	cPostProcess *pPostProcess = IsLoaded( Name );
	if ( !pPostProcess ) {
		cMsgCRT( "cPPManager::FadeOut:Cannot find post process %s", Name.c_str() );
		return false;
	}

	pPostProcess->SetActive( true );
	if ( pPostProcess->HasFadingProp() ) pPostProcess->SetVisibility( 1.0 );

	return true;
}


// Public
bool cPostProcessManager::Remove( std::string Name ) {

	if ( !bIsOK ) return false;

	cPostProcess *pPostProcess = IsLoaded( Name );
	if ( !pPostProcess ) {
		cMsgCRT( "cPPManager::FadeOut:Cannot find post process %s", Name.c_str() );
		return false;
	}

	pPostProcess->SetActive( false );
	if ( pPostProcess->HasFadingProp() ) pPostProcess->SetVisibility( 0.0 );

	return true;
}


// Public
void cPostProcessManager::Clear( void ) {

	pCamera->posteffects.clear();

}


// Public
void cPostProcessManager::Update( void ) {

	if ( !bIsOK ) return;


	for ( auto &pPair : mLoaded ) {

		// if time to remove it after fade out effect
		cPostProcess *pPostProcess = pPair.second;
		if ( !pPostProcess->Update() ) {

			// Set as inactive
			pPostProcess->SetActive( false );
		}
	}

}
