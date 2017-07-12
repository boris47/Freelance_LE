
#pragma once

// Public
void cPeriodicFunction::Update() {

	if ( !bIsOK ) return;
	if ( pCondition && !pCondition() ) return;
	if ( iTimer <= Leadwerks::Time::GetCurrent() ) {
		iTimer = Leadwerks::Time::GetCurrent() + iExecuteTime;
		pFunction();
	}

}

///////////////////////////////////////////////////////////////////////////


// Private
void cEngine::DrawInfos() {

	int Start = 240;
	std::list < std::string > ::iterator it = vInfoQueue.begin();
	for ( ; it != vInfoQueue.end(); it++ ) {
		pRenderer->DrawInfo( *it, 2, Start );  Start += 18;
	}

//	for ( auto Info : vInfoQueue ) {
//		pRenderer->DrawInfo( Info, 2, Start );  Start += 18;
//	}
	vInfoQueue.clear();

}

// Public
unsigned long int cEngine::RegisterPeriodicHook( long int Time, HookFunction fn, HookCondition cond ) {

	if ( ( Time < 1 ) || ( !fn ) ) return ZERO_INT;

	cPeriodicFunction *Function = new cPeriodicFunction( Time, fn, cond );
	vPeriodicHooks.push_back( Function );
	return Function->GetID();

}

// Public
void cEngine::UnRegisterPeriodicHook( unsigned long int ID ) {

	if ( ( ID < 1 ) || !vPeriodicHooks.size() ) return;
	size_t idx = 0;
	for ( auto pFunction : vPeriodicHooks ) {
		if ( pFunction->GetID() == ID ) {
			SAFE_DELETE( pFunction );
			vPeriodicHooks.erase( vPeriodicHooks.begin() + idx );
		}
		idx++;
	}
}

// Private
void cEngine::CallPeriodicHooks() {
	// iteratre vector and call hooks using lampda function
	std::for_each( vPeriodicHooks.begin(), vPeriodicHooks.end(), [&] ( auto &f ) { f->Update(); } );
}