
#include "FootstepManager.h"

cFootstepManager::cFootstepManager( std::string sSection ) {

	cMsgDBG( "cFootstepManager::Constructor" );

	cSection *FootStepsSection = LTXReader.GetSection( sSection );
	if ( !FootStepsSection ) {
		cMsgCRT( "cFootstepManager::cFootstepManager: FootStepsSection == NULL" );
		return;
	}

	size_t size = FootStepsSection->GetMultiSize( "Steps" );
	if ( !size ) {
		cMsgCRT( "cFootstepManager::cFootstepManager: \"Steps\" size == 0" );
		return;
	}
	cMsgDBG( "Cycle start" );

	for ( size_t s = 1; s < size; s++ ) {
		std::string sFolderName = FootStepsSection->GetMultiValue( "Steps", s, LTX_READER::VALUE_TYPES::STRING );
		if ( !( sFolderName.length() > 0 ) ) {
			cMsgCRT( "cFootstepManager::cFootstepManager(%s): FolderName Length is 0 for item %i", sSection, s );
			continue;
		}

		std::string sFilePath = SOUND_FILES_PATH + std::string("\\Steps\\") + sFolderName;

		// Get the last file Es: Metal6.wav
		std::string sLastFileName = ListFiles( sFilePath, ".wav" ).back();

		// create dot path
		std::string sFinal = String_Transform( std::string( "Steps\\" ) + sFolderName );

		// save the counter: Es: Gamedata.Sounds.Steps.Metal = 6
		mCounters[ sFinal ] = sLastFileName.back();
	}

	cMsgDBG( "cFootstepManager::Constructor OK" );
	bIsOK = true;

}

cFootstepManager::~cFootstepManager( void ) {

}

void cFootstepManager::Step( Leadwerks::PickInfo &Info, bool bCourched, bool bRunning ) {
	
	if ( !bIsOK ) return;
	
	// Default Value
	std::string sSurfaceType = "Default";

	if ( Info.surface && Info.surface->GetMaterial() ) {

		// .../materials/concrete/concrete.mat
		std::string sMaterial = Info.surface->GetMaterial()->path;

		// .../materials/concrete/concrete.mat -> concrete
		sSurfaceType = GetNameFromFilePath( sMaterial );

		// concrete -> Concrete
		String_CapitalizeFirstChar( sSurfaceType );

	}

	// Steps.Concrete.Concrete
	std::string sFinalPath = "Steps." + sSurfaceType + "." + sSurfaceType;

	if ( iCounter > mCounters[ sFinalPath ] ) iCounter = 1;
	
	// Steps.Concrete.ConcreteN
	std::string sFootstep = sFinalPath + std::to_string( iCounter );
	cAudioDevice *pAudioDevice = Engine()->AudioDevice();
	cAudioSrc *pSource = pAudioDevice->FindSource( sFootstep );
	if ( pSource ) {
		float fVolume = pAudioDevice->GetSoundsVolume();
		
		if ( bCourched ) fVolume *= 0.2;
		if ( bRunning )  fVolume *= 1.3;

//		pSource->SetVolume( fVolume );

		pAudioDevice->Stop( pSource );
		pAudioDevice->Play2D( pSource )->SetVolume( fVolume );
	}

	iCounter++;
}
