
#pragma once

#include "Engine.h"

class cEntity;

class cSector {

	private:

		Leadwerks::Model *			pModel		= NULL;

		std::vector < cEntity * >	vEntities;

		float						fRange		= ZERO_FLOAT;

		std::string					sName		= EMPTY_STRING;


	public:

		inline	std::string			Name()			{ return sName; }

		inline Leadwerks::Vec3		GetPosition()	{ return pModel->GetPosition(); }

				bool				ContainsEntity( unsigned long int ID );



};