
#include "Zoom.h"
#include "Game\Camera\CamManager.h"


// Constructor
cZoom::cZoom( Leadwerks::Camera *pCamera ) {

	fCamZoom = fCamZoomActual = fCamZoomOrig = pCamera->GetZoom();
	fCamViewDist = pCamera->GetRange().y;

}


// Public
void cZoom::Update() {

	Leadwerks::Window *pWindow = Leadwerks::Window::GetCurrent();
	if ( pWindow->MouseHit( Leadwerks::Key::RButton ) ) {
		bZoomed = !bZoomed;
		fCamZoom = ( bZoomed ) ? ( fCamZoom + 30 ) : fCamZoomOrig;
	}
	if ( fCamZoom != fCamZoomActual ) {
		fCamZoomActual = Leadwerks::Math::Curve( fCamZoom, fCamZoomActual, fCamZoom - fCamZoomActual );
		CamManager()->GetCamera()->SetZoom( fCamZoomActual );

		float fRatio = fCamZoomOrig / fCamZoom; // 1 at start, 0.n then
		float fNewVewDist = fCamViewDist + ( fCamViewDist - ( fCamViewDist * fRatio ) );
		CamManager()->GetCamera()->range.y = fNewVewDist;
	}
	
}


// Public
void cZoom::Reset() {
	fCamZoom = fCamZoomOrig;
}