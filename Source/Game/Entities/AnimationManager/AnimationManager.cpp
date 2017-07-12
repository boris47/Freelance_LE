
#include "AnimationManager.h"

cAnimationManager::cAnimationManager( Leadwerks::Entity* Entity ) {

	if ( Entity == NULL ) {
		Leadwerks::Debug::Error("cAnimationManager::cAnimationManager:Entity is NULL in constructor.");
	}
	pEntity = Entity;
	iFrameOffset = Leadwerks::Math::Random( 0, 1000 );

}

cAnimationManager::~cAnimationManager() {

	for ( auto pAnimation : vAnimations ) {
		delete pAnimation;
	}
	vAnimations.clear();

}

void cAnimationManager::SetAnimationSequence( const std::string& Sequence, const float Speed, const int BlendTime, const bool Mode, func_ptr EndHook ) {
	
	if ( Mode == false ) {
		if ( vAnimations.size() > 0 ) {
			cAnimation* pAnim = vAnimations.back();
			if ( ( pAnim->sSequence == Sequence ) && ( pAnim->fSpeed == Speed ) ) {
				pAnim->iBlendTime = BlendTime;
				return;
			}
		}
	}

	cAnimation* pAnimation = new cAnimation();
	pAnimation->iBlendStart = Leadwerks::Time::GetCurrent();
	pAnimation->iBlendFinish = pAnimation->iBlendStart + BlendTime;
	pAnimation->sSequence = Sequence;
	pAnimation->iLength = pEntity->GetAnimationLength( pAnimation->sSequence, true);
	pAnimation->fSpeed = Speed;
	pAnimation->bMode = Mode;
	pAnimation->pEndHook = EndHook;
	pAnimation->bEndOfSequenceReached = false;

	vAnimations.push_back( pAnimation );

}

void cAnimationManager::Update() {

	float fBlend, fFrame;
	bool bDoAnimation = false;
	long iCurrentTime = Leadwerks::Time::GetCurrent();
	int iMaxAnim = -1;
	for (size_t i = 0; i < vAnimations.size(); ++i) {
		cAnimation* pAnimation = vAnimations[i];

		if ( bDoAnimation == false ) {
			bDoAnimation = true;
			pEntity->LockMatrix();
		}

		//Division can result in a float, so cast numerator and denominator to float.
		fBlend = float( iCurrentTime - pAnimation->iBlendStart ) / float( pAnimation->iBlendFinish - pAnimation->iBlendStart );
		fBlend = Leadwerks::Math::Min( 1.0, fBlend );

		if ( pAnimation->bMode == false ) {
			fFrame = ( iCurrentTime * pAnimation->fSpeed ) + iFrameOffset;
		}
		else {
			fFrame = (iCurrentTime - pAnimation->iBlendStart) * pAnimation->fSpeed;

			if ( fFrame >= ( pAnimation->iLength - 1 ) ) {
				fFrame = pAnimation->iLength - 1;
				iMaxAnim = i + 1;
				if (pAnimation->bEndOfSequenceReached == false) {
					if ( pAnimation->pEndHook != nullptr ) {
						pAnimation->pEndHook( pEntity, pAnimation->sSequence );
						pAnimation->pEndHook = nullptr;
					}
					pAnimation->bEndOfSequenceReached = true;
				}
			}
		}

		pEntity->SetAnimationFrame( fFrame, fBlend, pAnimation->sSequence, true );

		if ( fBlend >= 1.0 ) iMaxAnim = Leadwerks::Math::Max( iMaxAnim, i );
	}

	if ( bDoAnimation == true ) pEntity->UnlockMatrix();

	if ( iMaxAnim > -1 )  {
		for (size_t n = 0; n < vAnimations.size(); ++n) {
			cAnimation* pCompletedAnimation = vAnimations[ n ];
			if ( int(n) < iMaxAnim ) {
				if ( ( pCompletedAnimation->bMode == false ) || ( pCompletedAnimation->bEndOfSequenceReached == true ) ) {
					vAnimations.erase( vAnimations.begin() + n );
					--n;
				}
			}
			else break;
		}
	}

}