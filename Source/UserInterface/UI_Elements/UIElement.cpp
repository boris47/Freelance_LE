

#include "UIElement.h"
#include "../UI_Manager.h"

#include "Leadwerks.h"

cUI_Element::cUI_Element( int iType, std::string sContent, int iX, int iY, int iWidth, int iHeight, Leadwerks::Widget *pBase ) {
	
	switch ( iType ) {
		
		case UI_Elements_Types::BUTTON: {
				pWidget = Leadwerks::Widget::Button(	sContent,	iX, iY, iWidth, iHeight, pBase );
		}
		case UI_Elements_Types::CHECKBOX: {
				pWidget = Leadwerks::Widget::Button(	sContent,	iX, iY, iWidth, iHeight, pBase );
				
#ifdef LAST_LE_VER

				pWidget->SetString( "style", "Checkbox" );
#else
				pWidget->SetStyle( 0 );

#endif // LAST_LE_VER
		}
		case UI_Elements_Types::LABEL: {
				pWidget = Leadwerks::Widget::Label(		sContent,	iX, iY, iWidth, iHeight, pBase );
		}
		case UI_Elements_Types::SLIDER: {
				pWidget = Leadwerks::Widget::Slider(				iX, iY, iWidth, iHeight, pBase );
		}
		case UI_Elements_Types::TEXTBOX: {
				pWidget = Leadwerks::Widget::TextArea(				iX, iY, iWidth, iHeight, pBase );
		}
		case UI_Elements_Types::TEXTFIELD: {
				pWidget = Leadwerks::Widget::TextField(	sContent,	iX, iY, iWidth, iHeight, pBase );
		}

	}
	
	this->iType = iType;

}
