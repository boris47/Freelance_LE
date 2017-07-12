
#pragma once

#include "Task.h"

#include <map>

class cTaskManager {
	private:
		std::map < int, cTask* > mTasks;
	
	public:
		cTask *AddTask( std::string Key );
		cTask *GetTask( std::string Key );
		cTask *GetTask( int ID );
		void RemTask( std::string Key );
		
		bool Update();
	
};