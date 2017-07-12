
#pragma once

#include <list>

class cInventoryObject {
	public:
		int Quantity;
		int Type;
		std::list <int> IDs;
		cInventoryObject( int Type, int Quantity );
	
};

class cInventory {
	private:
		std::list < cInventoryObject * > lObjects;
		
		bool ObjectCheck( std::string SectionName );
	
	public:
/*		bool Set( std::string SectionName, int Quantity = 1 );
		int Object( std::string SectionName );
		size_t InventorySize() { return lObjects.size(); };
		void Clear() { lObjects.clear(); };
	*/
};