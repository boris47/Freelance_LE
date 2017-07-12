
#pragma once

#include <string>
#include <unordered_map>


namespace INI_SYSTEM {
	namespace READ_PHASE {
		enum { NONE, SECTION };
	}
	namespace VALUE_TYPE {
		enum { NONE, INT, BOOL, FLOAT, STRING };
	}
}



#include "..\\Utils.h"
#include "INI_Section.h"

class cINI_System {
	private:

		std::unordered_map < std::string, cINI_Section * > mFileMap;
		std::set < std::string > sFilePaths;

		char iReadingPhase = INI_SYSTEM::READ_PHASE::NONE;

		bool bIsOK = false;

	public:
		
		cINI_System();
		~cINI_System();
		inline bool IsOK() { return bIsOK; };

	// Reading
	private:
		inline bool Section_Create ( cINI_Section *&Section, char *&cLine, std::string FilePath, int Line );
		inline bool Section_Insert ( cINI_Section *&Section, KeyValue_t &KeyValue, std::string FilePath, int NumLine );
		inline void Section_Close  ( cINI_Section *&Section );

	public:
		
		// Load a File
		inline bool				LoadFile( std::string FilePath );
		
		// Check if a file is loaded
		inline bool				Loaded( std::string FilePath );

		// Create a new section alreay stored !WARNING! if one already exist destroy it
		inline cINI_Section*	NewSection( std::string Name, cINI_Section* Inherited = NULL );

		// Get a existing section
		inline bool				GetSection( std::string Name, cINI_Section **Section = NULL );

		// Clear all sections
		inline void				Clear( void ) {
			for ( auto pPair : mFileMap ) delete pPair.second;
			mFileMap.clear();
		}

		// In debug mode print section map in a file
		void PrintMap( std::string FileName = "SectionMapFile.txt" );

};

void cINI_System::PrintMap( std::string FileName ) {

#if _DEBUG
	ofstream SectionMapFile;
	SectionMapFile.open( FileName );
	for ( auto pPair : mFileMap ) {
		cINI_Section *pSection = pPair.second;
		SectionMapFile << "\n[" + pSection->Name() + "]\n";
		for ( auto pPair : pSection->GetData() ) {
			SectionMapFile << pPair.first +" = " + pPair.second + "\n";
		}
	}
	SectionMapFile.close();
#endif

}

#include "INI_System_inline.h"

extern cINI_System		*pINI_System;
extern cINI_System		*pINI_Settings;

cINI_System *pINI_System	= NULL;
cINI_System *pINI_Settings	= NULL;

#define INI_System		*pINI_System
#define INI_Settings	*pINI_Settings