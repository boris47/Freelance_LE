
#include "UI_Manager.h"

#include "Engine.h"


cUI_Manager::cUI_Manager() {

	Leadwerks::Context *pContext = Leadwerks::Context::GetCurrent();

	if ( !( pGUI = Leadwerks::GUI::Create( pContext ) ) ) {
		cMsgCRT( "cUI_MainMenu::Constructor: Cannot create a gui elements" );
		return;
	}

	


	bIsOK = true;
}

bool cUI_Manager::Initialize() {
	bIsOK = false;
	pMainMenu = new cUI_MainMenu();
	if( !pMainMenu ) return false;
	bIsOK = true;
	return true;
}



void cUI_Manager::Update() {

	while ( Leadwerks::EventQueue::Peek() ) {
		Leadwerks::Event event = Leadwerks::EventQueue::Wait();
		if ( event.id == Leadwerks::Event::WidgetAction) {
			Leadwerks::System::Print( "WidgetAction" );
		}
		if ( event.id == Leadwerks::Event::WidgetSelect ) {
			Leadwerks::System::Print( "WidgetSelect" );
		}

		if ( event.id == Leadwerks::Event::WidgetMenu ) {
			Leadwerks::System::Print( "WidgetMenu" );
		}
	}

}