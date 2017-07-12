
#pragma once

#include "Leadwerks.h"
#include <string>

typedef void(*func_ptr)( Leadwerks::Entity* Entity, std::string& Sequence);

class cAnimation {
	public:
		long iBlendStart;
		long iBlendFinish;
		int iBlendTime;
		std::string sSequence;
		int iLength;
		float fSpeed;
		bool bMode;
		bool bEndOfSequenceReached = false;
		func_ptr pEndHook;
		
		cAnimation();
		~cAnimation();
	
};