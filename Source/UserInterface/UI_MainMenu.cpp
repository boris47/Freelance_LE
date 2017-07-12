


#include "UI_MainMenu.h"

#include "Engine.h"

cUI_MainMenu::cUI_MainMenu() {

	Leadwerks::Context *pContext = Leadwerks::Context::GetCurrent();

	Leadwerks::Widget *pBase = Engine()->UI_Manager()->GetGUI()->GetBase();

	vWidgets.push_back( new cUI_Element( UI_Elements_Types::BUTTON, "Play", pContext->GetWidth() / 2.0, pContext->GetHeight() / 2.0, 200, 400, pBase ) );
	
	Engine()->SetInternalState( EngineStates::MAINMENU );

	bIsOK = true;
}

void cUI_MainMenu::Show() {

	for each ( cUI_Element *pElement in vWidgets ) {
		pElement->GetWidget()->Show();
	}

}

void cUI_MainMenu::Hide() {

	for each ( cUI_Element *pElement in vWidgets ) {
		pElement->GetWidget()->Hide();
	}

}