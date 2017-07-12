

#include "Thunderbolt.h"

cThunderbolt::cThunderbolt( cAudioSrc * Source, int Delay, float Duration ) {

	pAudioSrc = Source;
	fDuration = Duration;
	iDelay = Delay;

}

void cThunderbolt::Update() {

	if ( !bIsActive ) return;





}
