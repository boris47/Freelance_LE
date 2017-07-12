
#include "MultiThread.h"

#include <stdlib.h>
#include <windows.h>

cCriticalSection::cCriticalSection() {

	pMutex = malloc( 1 );
	InitializeCriticalSection( ( CRITICAL_SECTION* ) pMutex );

}

cCriticalSection::~cCriticalSection() {

	DeleteCriticalSection( ( CRITICAL_SECTION* ) pMutex );
	free( pMutex );

}

void cCriticalSection::Enter() {

	EnterCriticalSection( ( CRITICAL_SECTION* ) pMutex );

}

void cCriticalSection::Leave() {

	LeaveCriticalSection( ( CRITICAL_SECTION* ) pMutex );

}

bool cCriticalSection::TryEnter() {

	return ( TryEnterCriticalSection( ( CRITICAL_SECTION* ) pMutex ) != 0 );

}