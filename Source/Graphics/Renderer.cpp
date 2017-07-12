
#include "Renderer.h"
#include "Game\Camera\CamManager.h"
#include "Game\Level\World.h"

// Public
cRenderer::cRenderer() {

	cSection *pSection = Settings.GetSection( "Video" );

	this->iWidth		= pSection->GetInt( "Width", 800 );
	this->iHeight		= pSection->GetInt( "Height", 600 );
	this->bFullscreen	= pSection->GetBool("Fullscreen", false );
	this->bVsync		= pSection->GetBool("Vsync", true );
	
	this->iWinFlags = Leadwerks::Window::Titlebar + Leadwerks::Window::Center;

	if ( this->bFullscreen ) {
		this->iWinFlags = Leadwerks::Window::FullScreen;
		Leadwerks::iVec2 gmode = Leadwerks::System::GetGraphicsMode( Leadwerks::System::CountGraphicsModes() - 1 );
		this->iWidth  = gmode.x;
		this->iHeight = gmode.y;
	}

	Leadwerks::GraphicsDriver * pGraphicDriver = Leadwerks::GraphicsDriver::GetCurrent();
	cMsgINFO( "Tesselletion Supported %s", pGraphicDriver->tessellationsupported ? "true" : "false" );
	cMsgINFO( "Occlusion Culling Supported %s", pGraphicDriver->occlusioncullingsupported ? "true" : "false" );

	cMsgDBG( "cRenderer::cRenderer() OK" );
	bIsOK = true;
}


// Public
cRenderer::~cRenderer() { SAFE_DELETE ( pContext ); SAFE_DELETE ( pWindow );  }


// Public
bool cRenderer::Initialize() {

	if ( !bIsOK ) {
		cMsgCRT( "cRenderer::Initialize:cRenderer bad initialization" );
		return false; // leave bIsInitialized == false
	}

	cMsgDBG( "cRenderer::Initialize()" );
	if ( !( pWindow = Leadwerks::Window::Create( "Freelance_1.0", 0, 0, iWidth, iHeight, iWinFlags ) ) ) {
		cMsgCRT( "cRenderer::Initialize:Failed to create window" );
		SAFE_DELETE( pWindow );
		return false; // leave bIsInitialized == false
	}
	
	if ( !( pContext = Leadwerks::Context::Create( pWindow ) ) ) {
		cMsgCRT( "cRenderer::Initialize:Failed to create context" );
		SAFE_RELEASE( pWindow );
		SAFE_DELETE( pContext );
		return false; // leave bIsInitialized == false
	}

	pWindow->SetMousePosition( iWidth, iHeight );
	pWindow->HideMouse();

	cMsgDBG( "cRenderer::Initialized" );
	bIsInitialized = true;
	return true;
}


// Public
void cRenderer::DrawStats() {

	if ( !bIsInitialized ) return;

	pContext->SetBlendMode( Leadwerks::Blend::Alpha );
	pContext->SetColor( 1, 1, 1, 1 );
	pContext->DrawStats( 20, 20 );
	pContext->SetBlendMode( Leadwerks::Blend::Solid );

}


// Public
void cRenderer::DrawInfo( std::string Info, int x, int y ) {

	if ( !bIsInitialized ) return;

	pContext->SetBlendMode( Leadwerks::Blend::Alpha );
	pContext->SetColor( 1, 1, 1, 1 );
	pContext->DrawText( Info, x, y );
	pContext->SetBlendMode( Leadwerks::Blend::Solid );

}


// Public
void cRenderer::Update() {

	if ( !bIsInitialized ) return;
	
	Leadwerks::Buffer *pBuffer = Leadwerks::Buffer::GetCurrent();
	int iWidth = pBuffer->GetWidth(), iHeight = pBuffer->GetHeight();

	// Draw 4 pixel as a crosshair
	pContext->DrawLine( iWidth/2-7, iHeight/2,   iWidth/2+8, iHeight/2   );		// Horiz
	pContext->DrawLine( iWidth/2,   iHeight/2-7, iWidth/2,   iHeight/2+8 );		// Vert

	// draw pointed distance
	char buffer [50];
	sprintf( buffer, "%.2f", CamManager()->GetPointedDistance() );
	std::string sDistance = std::string( buffer );
	pContext->SetBlendMode( Leadwerks::Blend::Alpha );
	pContext->DrawText( sDistance, iWidth/2 - pContext->GetFont()->GetTextWidth( sDistance )/2, iHeight/2 + 20 );
	pContext->SetBlendMode( Leadwerks::Blend::Solid );
	
	pContext->Sync( bVsync );
}


// Public
void cRenderer::Clear( void ) {

	if ( !bIsInitialized ) return;
	pContext->SetColor( 0.0f, 0.0f, 0.0f );	// Balck
	pContext->Clear();
}