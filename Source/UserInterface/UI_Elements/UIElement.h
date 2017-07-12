

#pragma once

#include "Leadwerks.h"


class cUI_Element {

	private:

		int iType = 0;
		Leadwerks::Widget *pWidget = nullptr;

	public:
		cUI_Element( int Type, std::string sContent, int iX, int iY, int iWidth, int iHeight, Leadwerks::Widget *pBase = NULL );

		inline	int						GetType()	{ return iType; }
		inline	Leadwerks::Widget *		GetWidget() { return pWidget; }

};