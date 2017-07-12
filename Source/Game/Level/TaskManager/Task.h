
#pragma once

enum { eCOMPLETED, eFAIL, eINPROGRESS };

#include "Objective.h"

#include <string>

class cTask {
	private:
		int iState = eINPROGRESS;
		cObjective *objective = NULL;
		std::string titile = std::string( "" );
		
	
	public:
		
		cObjective *GetObjective();
		std::string GetTitle();
		int GetObjectivesCount();
		
		bool Update();
	
};