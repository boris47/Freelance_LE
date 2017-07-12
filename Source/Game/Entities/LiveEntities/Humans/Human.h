
#pragma once

#include <Engine.h>
#include "Game\Entities\LiveEntities\LiveEntity.h"

class cHuman : public cLiveEntity {
	protected:
//		Leadwerks::SpotLight *pFlashlight	= NULL;
//		std::string sFlashlightSound		= "";
//		Leadwerks::Pivot *pWeaponTag		= NULL;

		struct Dragged {
			Leadwerks::Entity *	pEntity			= NULL;
			Leadwerks::Vec3		vPosition		= EMPTY_VEC3;
			Leadwerks::Vec3		vRotation		= EMPTY_VEC3;
			int					iCollisionOrig	= -1;
		} Dragged;
	
	public:
		inline	bool				IsDragging( void )			{ return Dragged.pEntity != NULL; }
				bool				SetDragging( Leadwerks::Entity *e );
		inline	Leadwerks::Entity *	GetEntityDragged( void )	{ return Dragged.pEntity; }
				void				DropEntityDragged( void );


		virtual void	Update( void )			= 0;
		virtual void	OnFrame( void )			= 0;

};
