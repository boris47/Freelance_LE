
#pragma once

class cCriticalSection {

	private:
		void* pMutex = nullptr;

	public:
		cCriticalSection();
		~cCriticalSection();

		void Enter();
		void Leave();
		bool TryEnter();

};