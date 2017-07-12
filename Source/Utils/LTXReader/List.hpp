#ifndef LIST_H
#define LIST_H
#pragma once

class cList {
	
	private:
		std::vector< cLineValue* > vList;
		std::string sName = "";

		bool bIsOK = false;
	
	public:
		
		cList( std::string Name, cList* Inherited = NULL ) {
			if ( Inherited ) vList = Inherited->GetData();
			sName = Name;
		}
		
		~cList( void ) {
			for ( auto pLineValue : vList )
				delete pLineValue;
			vList.clear();
		}
		
		inline bool 						IsOK() 			{ return bIsOK == false; }
		inline std::string					Name( void ) 	{ return sName; }
		
		inline void							Add( cLineValue *Line );
		inline std::vector< cLineValue* >	GetData( void ) { return vList; }
		
		
		inline void							Print( void );
	
};

void cList::Add( cLineValue *pLineValue ) {

	if ( pLineValue && pLineValue->IsOK() ) {
		vList.push_back( pLineValue );
	}
	
}

void cList::Print( void ) {
	
	for ( auto pLineValue : vList ) {
		cMsgDBG( "    {%s} = { ", pLineValue->Key().c_str() );
		pLineValue->Print();
		cMsgDBG( "    }");
	}
}

#endif//LIST_H