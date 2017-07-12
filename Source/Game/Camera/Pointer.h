

#pragma once

#include <engine.h>

class cPointer {

	private:
		
		// Ray cast source
		Leadwerks::Entity *pEntity = NULL;

		// Ray cast distance
		float fMaxDistance = ZERO_FLOAT;
		float fReachedDistance = ZERO_FLOAT;

		// Start Position
		Leadwerks::Vec3 vPosition = EMPTY_VEC3;

		// Direction
		Leadwerks::Vec3 vDirection = EMPTY_VEC3;
		
		// Collision info
		bool bHasInfo = false;
		Leadwerks::PickInfo Info = Leadwerks::PickInfo();

	public:
		cPointer( Leadwerks::Entity *SrcEntity = NULL );

				bool				SetSource( Leadwerks::Entity *Entity = NULL );
		inline	Leadwerks::Entity	*GetSource() { return pEntity; }

				bool				SetPosition( Leadwerks::Vec3 Position );
		inline	Leadwerks::Vec3		Getposition() { return vPosition; }

				bool				SetDirection( Leadwerks::Vec3 Direction );
		inline	Leadwerks::Vec3		GetDirection() { return vDirection; }

				bool				SetMaxDistance( float MaxDistance );
		inline	float				GetDistance() { return fReachedDistance; }

		inline	bool				HasInfo() { return bHasInfo; }
		inline	Leadwerks::PickInfo	GetInfo() { return Info; }

		void	Update();

		void	Reset();

};