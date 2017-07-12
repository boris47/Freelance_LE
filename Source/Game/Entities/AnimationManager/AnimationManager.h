
#pragma once

#include "Leadwerks.h"
#include "Animation.h"

#include <string>
#include <vector>

class cAnimationManager {
	private:
		Leadwerks::Entity* pEntity;
		short iFrameOffset;
		std::vector< cAnimation* > vAnimations;
	
	public:
		cAnimationManager( Leadwerks::Entity* Entity );
		virtual ~cAnimationManager();
		virtual void SetAnimationSequence( const std::string& Sequence, const float Speed = 1.0f, const int BlendTime = 500, const bool Mode = false, func_ptr EndHook = nullptr );
		virtual void Update();
		void ClearAnimations();
	
};