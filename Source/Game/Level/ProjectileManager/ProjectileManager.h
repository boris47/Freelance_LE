
#pragma once

#include "Projectile.h"
#include <list>

class cProjectileManager {
	private:
		std::list < cProjectile* > lProjectiles;
	
	public:
		cProjectile *AddProjectile( cProjectile *Projectile );
		void RemProjectile( int ID );
		
		bool Update();

		void Reset() {}
	
};