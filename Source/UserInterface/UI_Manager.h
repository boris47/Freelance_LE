
#pragma once

namespace UI_Elements_Types {

	const int BUTTON		= 0;
	const int CHECKBOX		= 1;
	const int LABEL			= 2;
	const int TEXTBOX		= 3;
	const int TEXTFIELD		= 4;
	const int SLIDER		= 5;

};


#include "UI_Elements\CheckBox.h"
#include "UI_Elements\Label.h"
#include "UI_Elements\Panel.h"
#include "UI_Elements\Slider.h"
#include "UI_Elements\TextBox.h"

#include "UI_Elements\UIElement.h"

// Menus
#include "UI_MainMenu.h"

class cUI_Manager {
	private:
		
		Leadwerks::GUI *pGUI = nullptr;

		cUI_MainMenu *pMainMenu = nullptr;

		bool bIsOK = false;

	
	public:

		cUI_Manager();

				bool	Initialize();

		inline	bool	IsOK()			{ return bIsOK; }


		inline	Leadwerks::GUI	*GetGUI() { return pGUI; }
		
		// Main Menu
		inline	void	ShowMainMenu() { pMainMenu->Show(); }
		inline	void	HideMainMenu() { pMainMenu->Hide(); }
		
		void Update();
	
};