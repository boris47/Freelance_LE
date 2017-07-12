
#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>


// Cut off every comment, all is behind a semicolon
void CleanComments( std::string &String ) {
	if ( String.empty() ) return;
	for ( size_t Index = 0; Index < String.size(); Index++ ) {
		if ( String[ Index ] == ';' ) {
			String.erase( Index );
			break;
		}
	}
}

bool ValidString( std::string &String ) {
	
	if ( String.empty() ) return false;

	CleanComments( String );

	if ( String.empty() ) return false;
	
	return true;
}


// Private
bool cINI_System::Section_Create( cINI_Section *&Section, char *&cLine, std::string FilePath, int Line ) {
	
	// if previous open then save it
	this->Section_Close( Section );

	// Get the name of mother section, if present
	cINI_Section *InheritedSection = NULL;
	LPCSTR InheritedName = strstr( cLine, ":" );
	if ( 0 != InheritedName ) {
		InheritedName += 1;
		if ( !( GetSection( std::string( InheritedName ), &InheritedSection ) ) ) {
			cMsgCRT ( "cLTXReader::Open_NewSection:%s:[%i]: Section requested for inheritance \"%s\" doesn't exist!", FilePath.c_str(), Line, InheritedName );
			return false;
		}
	}

	// get the name of the section
	*strchr( cLine, ']' ) = 0;
	std::string SectionName = std::string( cLine + 1 );
	if ( GetSection( SectionName, NULL ) ) {
		cMsgCRT ( "cLTXReader::Open_NewSection:%s:[%i]: Section \"%s\" already exists!", FilePath.c_str(), Line, SectionName.c_str() );
		SAFE_DELETE( Section );
		return false;
	}

	iReadingPhase = INI_SYSTEM::READ_PHASE::SECTION;

	// finally create it
	Section = new cINI_Section( SectionName, InheritedSection );
	return true;
}


// Private
bool cINI_System::Section_Insert( cINI_Section *&Section, KeyValue_t &KeyValue, std::string FilePath, int NumLine ) {

	if ( !Section->AddKeyValue( KeyValue.Key, KeyValue.Value ) ) {
		cMsgCRT( "cINI_System::Section_Insert:invalid line in file %s, at line %d", FilePath.c_str(), NumLine );
		return false;
	}
	return true;

}


// Private
void cINI_System::Section_Close( cINI_Section *&Section ) {

	if ( Section ) {
		mFileMap[ Section->Name() ] = Section;
		Section = NULL;
	}
	iReadingPhase = INI_SYSTEM::READ_PHASE::NONE;

}



// Public
bool cINI_System::LoadFile( std::string FilePath ) {

	if ( !FileExists( FilePath ) ) {
		cMsgCRT( "cLTXReader::LoadFile:File \"%s\" doesn't exist", FilePath.c_str() );
		return false;
	}
	
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
	cINI_Section *pSection = NULL;

	while( FileHandler.good() ) {
		iLine++;
		std::string Line;
		std::getline( FileHandler, Line );
		if ( !ValidString( Line ) ) continue;

		char *cLine = ( char * )Line.data();

		// INCLUSION
		/// Able to include file in same dir of root file
		if ( cLine[ 0 ] && ( cLine[ 0 ] == '#' ) && strstr( cLine, "#include" ) ) {
			std::string Path = GetPathFromFilePath( FilePath );
			std::string FileName = Line.substr( 9, std::string::npos );
			if ( !LoadFile( Path + FileName ) ) return false;
			continue;
		}

		// SECTION CREATION
		if ( cLine[ 0 ] && ( cLine[ 0 ] == '[' ) ) {	// Create a new section
			Section_Create( pSection, cLine, FilePath, iLine );
			continue;
		}

		// LINE INSERTION
		KeyValue_t KeyValue = GetKeyValue( Line );
		if ( KeyValue.IsOK ) {
			if ( !pSection ) {
				cMsgCRT ( " cLTXReader::LoadFile:No section created to insert KeyValue at line |%d| in file |%s| ", iLine, FilePath.c_str() );
				SAFE_DELETE( pSection );
				return false;
			}

			if ( iReadingPhase != INI_SYSTEM::READ_PHASE::SECTION ) {
				cMsgCRT ( " cLTXReader::LoadFile:Trying to insert a KeyValue into a non section type FileElement, line \"%s\" of file %s!", Line.c_str(), FilePath.c_str() );
				continue;
			}

			if ( !Section_Insert( pSection, KeyValue, FilePath, iLine ) ) return false;
			continue;
		}

		// NO CORRECT LINE DETECTED
		cMsgCRT( "cLTXReader::LoadFile:Incorrect line %i in file %s", iLine, FilePath.c_str() );
		SAFE_DELETE( pSection );
		return false;
	}

	Section_Close( pSection );

	sFilePaths.insert( FilePath );
	cMsgDBG( "cLTXReader::LoadFile:Loaded file %s", FilePath.c_str() );
	return true;

}

// Public
bool cINI_System::Loaded( std::string FilePath ) {
	return ( sFilePaths.find( FilePath ) != sFilePaths.end() );
}

// Public
cINI_Section* cINI_System::NewSection( std::string Name, cINI_Section* Inherited ) {

	cINI_Section *pSection = NULL;
	if ( GetSection( Name, &pSection ) ) delete pSection;

	cINI_Section *pSection = new cINI_Section( Name, Inherited );
	mFileMap[ Name ] = pSection;
	return pSection;

}

// Public
bool cINI_System::GetSection( std::string Name, cINI_Section **Section ) {

	if ( mFileMap.empty() == true ) return false;
	try { *Section = mFileMap.at( Name ); }
	catch ( ... ) { return false; }
	return true;

}