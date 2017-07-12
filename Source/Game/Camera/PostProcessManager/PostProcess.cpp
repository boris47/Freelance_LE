

#include "PostProcess.h"
#include <Engine.h>


// Constructor
cPostProcess::cPostProcess( std::string FilePath, float StartFadeEffect ) : fFadeEffect( StartFadeEffect ), bActive( false ) {

	pShader = Leadwerks::Shader::Load( FilePath );
	if ( pShader == NULL ) {
		cMsgCRT( "cPostProcess::cPostProcess:Failed loading shader %s", FilePath.c_str() );
		return; // leave bIsOK false
	}

	bFadingProp = ( pShader->GetUniform( "fFadeEffect" ) != NULL );
	if ( bFadingProp )
		pShader->SetFloat( "fFadeEffect", StartFadeEffect );

	bIsOK = true;
}


// Constructor
cPostProcess::cPostProcess( Leadwerks::Shader *Shader ) : bActive( false ) {

	if ( Shader == NULL ) return;

	pShader = Shader;

	bIsOK = true;

}


// Desctructor
cPostProcess::~cPostProcess() {
	if ( pShader ) pShader->Release();
}


// Public
cPostProcess *cPostProcess::SetActive( bool Active ) {

	bActive = Active;

	if ( Active )
		pShader->Enable();
	else {
		iFadeTime = iTime = 0;
		bToFade = false;
		pShader->Disable();
	}

	return ( this );
}


// Public
cPostProcess *cPostProcess::SetFadeTime( long Time ) {

	if ( !bFadingProp ) return ( this );

	if ( Time > -1 ) {
		iTime = Time;
		iFadeTime = Leadwerks::Time::GetCurrent() + Time;
		bToFade = true;
	}

	return ( this );
}


// Public
cPostProcess *cPostProcess::SetVisibility( float Value ) {

	if ( !bFadingProp ) return ( this );

	fFadeEffect = Leadwerks::Math::Clamp( Value, 0.0, 1.0 );
	pShader->SetFloat( "fFadeEffect", fFadeEffect );

	return ( this );
}


// Public
bool cPostProcess::Update( void ) {

	if ( !bIsOK ) return false;

	if ( bFadingProp ) {

		if ( bToFade ) {
			long iDeltaTime = iFadeTime - Leadwerks::Time::GetCurrent();

			float fOperation = float( iDeltaTime ) / float( iTime );
			fFadeEffect = (bFadeIn) ? ( 1 - fOperation ) : fOperation;

			if ( iDeltaTime <= 0 ) {
				bToFade = false;
				pShader->SetFloat( "fFadeEffect", ( bFadeIn ) ?1.0:0.0 );
				return ( bFadeIn ) ? true:false;
			} // stop fading effect
		}
		pShader->SetFloat( "fFadeEffect", fFadeEffect );
	}

	return true;	
}