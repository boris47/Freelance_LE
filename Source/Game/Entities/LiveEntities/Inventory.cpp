
#include "Inventory.h"
/*
bool CInventory::ObjectCheck( std::string SectionName ) {
	try { mInventoryMap.at( SectionName ); }
	catch ( ... ) { return false; }
	return true;
}


bool CInventory::Set( std::string SectionName, int Quantity ) {
	if ( !ObjectCheck( SectionName, "Set" ) ) return false;
	
	if ( Quantity <= 0 ) {
		Remove( SectionName );
		return true;
	}
	
	mInventoryMap[ SectionName ] = Quantity;
	return true;
}
int CInventory::Object( std::string SectionName ) {
	if ( !ObjectCheck( SectionName, "Object" ) ) return -1;
	return mInventoryMap.at( SectionName );
}*/