
#pragma once

cActor *cLevel::GetActor() {
	return ( !bIsInitialized || !bIsPresent ) ? NULL : ( dynamic_cast< cActor * > ( pActor ) );
}

cEntity *cLevel::GetActorAsEntity() {
	return ( !bIsInitialized || !bIsPresent ) ? NULL : pActor;
}