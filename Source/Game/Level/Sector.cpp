

#include "Sector.h"

#include "Game\Entities\Entity.h"


bool cSector::ContainsEntity( unsigned long int ID )  {

	for ( auto pEntity : vEntities ) if ( pEntity->ID() == ID ) return true;
	return false;

}