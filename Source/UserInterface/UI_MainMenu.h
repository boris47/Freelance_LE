
#pragma once

#include <vector>

class cUI_Element;

class cUI_MainMenu {

	private:

		std::vector< cUI_Element * > vWidgets;

		bool bIsOK = false;

	public:

		cUI_MainMenu();

		inline	bool	IsOK()				{ return bIsOK; }

				void	Show();
				void	Hide();

};