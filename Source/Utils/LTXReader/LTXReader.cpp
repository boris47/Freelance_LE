
///////////////////////////////////////////////
/// was all the input successfully consumed ///
/// return ( iss.rdbuf()->in_avail() == 0 ) ///
///////////////////////////////////////////////

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "LTXReader.hpp"

/*
	FILE = {
		FILEELEMENT(Section1) -> SECTION -> SECTION METHODS
		FILEELEMENT(Section2) -> SECTION -> SECTION METHODS
		FILEELEMENT(Section3) -> SECTION -> SECTION METHODS
		FILEELEMENT(List1) -> LIST -> LIST METHODS
		FILEELEMENT(List2) -> LIST -> LIST METHODS
	}
*/

cLTXReader *pLTXReader		= NULL;
cLTXReader *pLTXSettings	= NULL;

///////////////// ///////////////// ///////////////// /////////////////
///////////////// ///////////////// ///////////////// /////////////////
///////////////// ///////////////// ///////////////// /////////////////

// Constructor
cLTXReader::cLTXReader( std::string FilePath ) {

	if ( !LoadFile( FilePath ) ) {
		cMsgCRT( "cLTXReader::cLTXReader:Cannot load \"%s\"", FilePath.c_str() );
		return;
	}
	bIsOK = true;

}

// Destructor
cLTXReader::~cLTXReader() {

	for ( auto &pPair : mFileMap ) 
		SAFE_DELETE( pPair.second );

	mFileMap.clear();
	SAFE_DELETE( pDefaultCValue );

}


// Private
bool cLTXReader::Section_Create( cSection *&Section, cList *&List, char *&cLine, std::string FilePath, int Line ) {
	
	// if previous open then save it
	this->Section_Close( Section );
	this->List_Close(List );

	// Get the name of mother section, if present
	cSection *pInheritedSection = NULL;
	LPCSTR InheritedName = strstr( cLine, "]:" );
	if ( 0 != InheritedName ) {
		InheritedName += 2;
		if ( !( pInheritedSection = GetSection( std::string( InheritedName ) ) ) ) {
			cMsgCRT ( "cLTXReader::Section_Create:%s:[%i]: Section requested for inheritance \"%s\" doesn't exist!", FilePath.c_str(), Line, InheritedName );
			return false;
		}
	}

	// get the name of the section
	*strchr( cLine, ']' ) = 0;
	std::string SectionName = std::string( cLine + 1 );
	if ( HasFileElement( SectionName, NULL ) ) {
		cMsgCRT ( "cLTXReader::Section_Create:%s:[%i]: Section \"%s\" already exists!", FilePath.c_str(), Line, SectionName.c_str() );
		SAFE_DELETE( Section );
		return false;
	}

	// Create section and set new phase
	iReadingPhase = LTX_READER::READ_PHASE::SECTION;
	Section = new cSection( SectionName, pInheritedSection );

	return true;
}

// Private
bool cLTXReader::Section_Insert( cSection *&Section, cList *&List, KeyValue_t &KeyValue, std::string FilePath, int Line ) {

	cLineValue *pLineValue = new cLineValue( String_Trim( KeyValue.Key ), String_Trim( KeyValue.Value ) );
	if ( !pLineValue || !pLineValue->IsOK() ) {
		cMsgCRT ( " cLTXReader::Section_Insert:LineValue invalid for key |%s| in Section |%s| in file |%s|", KeyValue.Key.c_str(), Section->Name().c_str(), FilePath.c_str() );
		SAFE_DELETE( Section ); SAFE_DELETE( pLineValue ); SAFE_DELETE( List );
		return false;
	}

	Section->Add( pLineValue );
	return true;
}

// Private
void cLTXReader::Section_Close( cSection *&Section ) {

	if ( Section && ( !HasFileElement( Section->Name(), NULL ) ) ) {
		cFileElement *pFileElement = new cFileElement( LTX_READER::FILE_ELEMENT::SECTION );
		pFileElement->SetSection( Section );
		mFileMap[ Section->Name() ] = pFileElement;
		Section = NULL;
	}
	iReadingPhase = LTX_READER::READ_PHASE::NONE;
}

// Private
bool cLTXReader::List_Create( cList *&List, cSection *&Section, char *&cLine, std::string FilePath, int Line ) {

	// if previous open then save it
	this->Section_Close( Section );
	this->List_Close( List );

	// Get the name of mother list, if present
	cList *InheritedList = NULL;
	LPCSTR InheritedName = strstr( cLine, "}:" );
	if ( 0 != InheritedName ) {
		InheritedName += 2;
		if ( !( InheritedList = GetList( std::string( InheritedName ) ) ) ) {
			cMsgCRT ( "cLTXReader::Open_NewList:%s:[%i]: List requested for inheritance \"%s\" doesn't exist!", FilePath.c_str(), Line, InheritedName );
			return false;
		}
	}

	// get the name of the list
	*strchr( cLine, '}' ) = 0;
	std::string ListName = std::string( cLine + 1 );
	if ( HasFileElement( ListName, NULL ) ) {
		cMsgCRT ( "cLTXReader::Open_NewList:%s:[%i]: list \"%s\" already exists!", FilePath.c_str(), Line, ListName.c_str() );
		SAFE_DELETE( List );
		return false;
	}

	iReadingPhase = LTX_READER::READ_PHASE::LIST;

	// finally create it
	List = new cList( ListName, InheritedList );
	return true;
}

// Private
bool cLTXReader::List_Insert( cList *&List, cSection *&Section, KeyValue_t &KeyValue, std::string FilePath, int Line ) {

	cLineValue *pLineValue = new cLineValue( KeyValue.Key, KeyValue.Value );
	if ( !pLineValue || !pLineValue->IsOK() ) {
		cMsgCRT ( " cLTXReader::List_Insert:LineValue invalid for key |%s| in Section |%s| in file |%s| ", KeyValue.Key.c_str(), List->Name().c_str(), FilePath.c_str() );
		SAFE_DELETE( List ); SAFE_DELETE( pLineValue );
		return false;
	}

	List->Add( pLineValue );
	return true;
}

// Private
void cLTXReader::List_Close( cList *&List ) {
	if ( List && !HasFileElement( List->Name(), NULL ) ) {
		cFileElement *pFileElement = new cFileElement( LTX_READER::FILE_ELEMENT::LIST );
		pFileElement->SetList( List );
		mFileMap[ List->Name() ] = pFileElement;
		List = NULL;
	}

	iReadingPhase = LTX_READER::READ_PHASE::NONE;
}

// Public
bool cLTXReader::LoadFile( std::string FilePath ) {
	if ( !FileExists( FilePath ) ) {
		cMsgCRT( "cLTXReader::LoadFile:File \"%s\" doesn't exist", FilePath.c_str() );
		return false;
	}

	if ( Loaded( FilePath ) ) return true;
	
	std::ifstream FileHandler( FilePath.c_str() );
	if( !FileHandler.is_open() ) {	//if not opened, exit
		cMsgCRT( "cLTXReader::LoadFile:Cannot read \"%s\"", FilePath.c_str() );
		return false;
	}
	
	if ( FileHandler.peek() == std::ifstream::traits_type::eof() ) {
		cMsgCRT( "cLTXReader::LoadFile:Empty file \"%s\"", FilePath.c_str() );
		return false;
	}
	
	int iLine = 0;
	cSection *pSection = NULL;
	cList *pList = NULL;

	while( FileHandler.good() ) {
		iLine++;
		std::string Line;
		std::getline( FileHandler, Line );
		if ( !String_IsValid( Line ) ) continue;

		char *cLine = ( char * )Line.data();

		// INCLUSION
		/// Able to include file in same dir of root file
		if ( cLine[ 0 ] && ( cLine[ 0 ] == '#' ) && strstr( cLine, "#include" ) ) {
			std::string Path = GetPathFromFilePath( FilePath );
			std::string FileName = String_Trim( Line ).substr( std::string( "#include" ).length() );
			if ( !LoadFile( Path + FileName ) ) return false;
			continue;
		}

		// SECTION CREATION
		if ( cLine[ 0 ] && ( cLine[ 0 ] == '[' ) ) {	// Create a new section
			Section_Create( pSection, pList, cLine, FilePath, iLine );
			continue;
		}

		// LIST CREATION
		if ( cLine[ 0 ] && ( cLine[ 0 ] == '{' ) ) {	// Create a new list
			List_Create( pList, pSection, cLine, FilePath, iLine );
			continue;
		}

		// Line = "Key=Value;" or "ListMember;"
		
		// INSERTION
		KeyValue_t KeyValue = GetKeyValue( Line );
		if ( KeyValue.IsOK ) {
			if ( KeyValue.iType == TYPE_KEYVALUE_KEYVALUE ) {
				if ( !pSection ) {
					cMsgCRT ( " cLTXReader::LoadFile:No section created to insert KeyValue at line |%d| in file |%s| ", iLine, FilePath.c_str() );
					SAFE_DELETE( pSection ); SAFE_DELETE( pList );
					return false;
				}

				if ( iReadingPhase != LTX_READER::READ_PHASE::SECTION ) {
					cMsgCRT ( " cLTXReader::LoadFile:Trying to insert a KeyValue into a non section type FileElement, line \"%s\" of file %s!", Line.c_str(), FilePath.c_str() );
					continue;
				}

				if ( !Section_Insert( pSection, pList, KeyValue, FilePath, iLine ) ) return false;
				continue;
			}

			if ( KeyValue.iType == TYPE_KEYVALUE_LISTKEY ) {
				if ( !pList ) {
					cMsgCRT ( " cLTXReader::LoadFile:No list created to insert key at line |%d| in file |%s| ", iLine, FilePath.c_str() );
					SAFE_DELETE( pSection ); SAFE_DELETE( pList );
					return false;
				}
				
				if ( iReadingPhase != LTX_READER::READ_PHASE::LIST ) {
					cMsgCRT ( " cLTXReader::LoadFile:Trying to insert a KeyValue into a non list type FileElement, line \"%s\" of file %s!", Line.c_str(), FilePath.c_str() );
					continue;
				}

				if ( !List_Insert( pList, pSection, KeyValue, FilePath, iLine ) ) return false;
				continue;
			}
		}

		// NO CORRECT LINE DETECTED
		cMsgCRT( "cLTXReader::LoadFile:Incorrect line %i in file %s", iLine, FilePath.c_str() );
		SAFE_DELETE( pSection ); SAFE_DELETE( pList );
		return false;
	}
	
	Section_Close( pSection );
	List_Close( pList );

	sFilePaths.insert( FilePath );
	cMsgDBG( "cLTXReader::LoadFile:Loaded file %s", FilePath.c_str() );
	return true;
};


bool cLTXReader::HasFileElement( std::string Key, cFileElement **FileElement ) {
	
	if ( mFileMap.empty() == true ) return false;
	
	try { *FileElement = mFileMap.at( Key ); }
	catch ( ... ) { return false; }
	return true;
	
};


bool cLTXReader::Loaded( std::string FilePath ) {
	
	if ( sFilePaths.find( FilePath ) != sFilePaths.end() ) return true;
	
	return false;
	
}


cSection* cLTXReader::NewSection( std::string Name, cSection *Inherited ) {
	
	cFileElement *pFileElement = NULL;
	if ( HasFileElement( Name, &pFileElement ) ) delete pFileElement;
	cSection *pSection = new cSection( Name, Inherited );
	pFileElement = new cFileElement( LTX_READER::FILE_ELEMENT::SECTION );
	pFileElement->SetSection( pSection );
	return pSection;
	
}


cSection *cLTXReader::GetSection( std::string Name ) {
	
	cFileElement *pFileElement = NULL;
	if ( HasFileElement( Name, &pFileElement ) ) {
		return pFileElement->GetSection();
	}
	cMsgCRT( "cLTXReader::GetSection:Section \"%s\" not found!!", Name.c_str() );
	return NULL;
	
}


cList* cLTXReader::NewList( std::string Name, cList *Inherited ) {
	
	cFileElement *pFileElement = NULL;
	if ( HasFileElement( Name, &pFileElement ) ) delete pFileElement;
	cList *pList = new cList( Name, Inherited );
	pFileElement = new cFileElement( LTX_READER::FILE_ELEMENT::LIST );
	pFileElement->SetList( pList );
	return pList;
	
}


cList *cLTXReader::GetList( std::string Name ) {
	
	cFileElement *pFileElement = NULL;
	if ( HasFileElement( Name, &pFileElement ) ) {
		return pFileElement->GetList();
	}
	cMsgCRT( "cLTXReader::GetList:List \"%s\" not found!!", Name.c_str() );
	return NULL;
	
}


void cLTXReader::PrintMap( std::string FileName ) {
#if _DEBUG
	if ( !FileName.length() ) return;
	ofstream SectionMapFile;
	SectionMapFile.open( FileName );
	for ( const auto &pPair : mFileMap ) {
		if ( pPair.second->GetType() == LTX_READER::FILE_ELEMENT::SECTION ) {
			cSection *pSection = pPair.second->GetSection();
			if ( pSection ) {
				SectionMapFile << "\n[" + pSection->Name() + "]\n";
				std::vector< cLineValue* > vValues = pSection->GetData();
				for ( auto pLineValue : vValues ) {
					switch( pLineValue->Type() ) { 
						case LTX_READER::VALUE_TYPE::SINGLE : {
							SectionMapFile << pLineValue->Key().append( " = " + pLineValue->GetValue()->ToString() + "\n" );
							break;
						}
						case LTX_READER::VALUE_TYPE::MULTI : {
							std::string sMulti = pLineValue->Key() + " = ";
							for( auto it = pLineValue->GetMultiValue()->Begin(); it != pLineValue->GetMultiValue()->End(); it++ ) {
								sMulti.append( (*it)->ToString() + ", " );
							} sMulti.erase( sMulti.end() - 2 ); // remove last colon
							SectionMapFile << sMulti << "\n";
						}
						default: break;
					}
				}
			} 
		}

		if ( pPair.second->GetType() == LTX_READER::FILE_ELEMENT::LIST ) {
			cList *pList = pPair.second->GetList();
			if ( pList ) {
				SectionMapFile << "\n{" + pList->Name() + "}\n";
				std::vector< cLineValue* > vValues = pList->GetData();
				for ( auto pLineValue : vValues ) {
					SectionMapFile << pLineValue->Key()+ ";\n";
				}
			}
		}

	}
	SectionMapFile.close();
#endif
}

