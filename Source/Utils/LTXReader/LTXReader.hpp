#ifndef FILEREADER_H
#define FILEREADER_H
#pragma once



#include <string>
#include <map>
#include <set>
#include <vector>


#include "Leadwerks.h"

#include "..\Utils.h"
#include "Defines.hpp"
#include "..\..\Defines.h"

#include "Value.hpp"
#include "MultiValue.hpp"
#include "LineValue.hpp"
#include "Section.hpp"
#include "List.hpp"
#include "FileElement.hpp"

class cLTXReader {
	private:
		std::map < std::string, cFileElement * > mFileMap;
		typedef std::map < std::string, cFileElement * > FileMapType;
		typedef std::map < std::string, cSection * >::iterator mFileMapI;
		std::set < std::string > sFilePaths;
		
		cValue * pDefaultCValue = new cValue();

		char iReadingPhase = LTX_READER::READ_PHASE::NONE;

		bool bLUA_Allocated 	= false;
		bool bIsOK 				= false;

	public:
		cLTXReader() : bIsOK( true ) {}
		cLTXReader( std::string FilePath );
		~cLTXReader( void );
		inline 	void 		SetLUA_Allocated( bool b ) 	{ bLUA_Allocated = b; }
		inline 	bool 		IsLUA_Allocated() 			{ return bLUA_Allocated; }
		inline 	bool 		IsOK() 						{ return bIsOK; };
		
		inline 	FileMapType GetData( void ) 			{ return mFileMap; }

	private:
		bool Section_Create ( cSection *&Section, cList *&List, char *&cLine, std::string FilePath, int Line );
		bool Section_Insert ( cSection *&Section, cList *&List, KeyValue_t &KeyValue, std::string FilePath, int Line );
		void Section_Close  ( cSection *&Section );

		bool List_Create    ( cList *&List, cSection *&Section, char *&cLine, std::string FilePath, int Line );
		bool List_Insert    ( cList *&List, cSection *&Section, KeyValue_t &KeyValue, std::string FilePath, int Line );
		void List_Close     ( cList *&List );
		
	public:
		// Load a File
				bool		LoadFile( std::string FilePath );
		
		// Check if a section or a list with this name is loaded
				bool		HasFileElement( std::string Key, cFileElement **FileElement );

		// Check if a file is loaded
				bool		Loaded( std::string FilePath );
		
		// Create a new section alreay stored !WARNING! if one already exist destroy it
				cSection*	NewSection( std::string Name, cSection* Inherited = NULL );

		// Get a existing section
				cSection*	GetSection( std::string Name );

		// Create a new section alreay stored !WARNING! if one already exist destroy it
				cList*		NewList( std::string Name, cList* Inherited = NULL );

		// Get a existing list
				cList*		GetList( std::string Name );
		
		// In debug mode print section map in a file
				void 		PrintMap( std::string FileName = "SectionMapFile.txt" );
};

extern cLTXReader* pLTXReader;
extern cLTXReader* pLTXSettings;
#define LTXReader (*pLTXReader)
#define Settings (*pLTXSettings)

// Utility Defines for multi value read

#define GetMulti1(sec,name,idx,type)				sec->GetMultiValue( name, idx, type )

#define GetMulti2(sec,name,idx1, idx2, type)		sec->GetMultiValue( name, idx1, type ),\
													sec->GetMultiValue( name, idx2, type )

#define GetMulti3(sec,name,idx1, idx2, idx3, type)	sec->GetMultiValue( name, idx1, type ),\
													sec->GetMultiValue( name, idx2, type ),\
													sec->GetMultiValue( name, idx3, type )


#define GetMultiInt1(sec,name,idx)					GetMulti1( sec, name, idx,  LTX_READER::VALUE_TYPES::INT )
#define GetMultiInt2(sec,name,idx1, idx2)			GetMulti2( sec, name, idx1, idx2, LTX_READER::VALUE_TYPES::INT )
#define GetMultiInt3(sec,name,idx1, idx2, idx3)		GetMulti3( sec, name, idx1, idx2, idx3, LTX_READER::VALUE_TYPES::INT )

#define GetMultiFloat1(sec,name,idx)				GetMulti1( sec, name, idx,  LTX_READER::VALUE_TYPES::FLOAT )
#define GetMultiFloat2(sec,name,idx1, idx2)			GetMulti2( sec, name, idx1, idx2, LTX_READER::VALUE_TYPES::FLOAT )
#define GetMultiFloat3(sec,name,idx1, idx2, idx3)	GetMulti3( sec, name, idx1, idx2, idx3, LTX_READER::VALUE_TYPES::FLOAT )

#define GetMultiBool1(sec,name,idx)					GetMulti1( sec, name, idx,  LTX_READER::VALUE_TYPES::BOOL )
#define GetMultiBool2(sec,name,idx1, idx2)			GetMulti2( sec, name, idx1, idx2, LTX_READER::VALUE_TYPES::BOOL )
#define GetMultiBool3(sec,name,idx1, idx2, idx3)	GetMulti3( sec, name, idx1, idx2, idx3, LTX_READER::VALUE_TYPES::BOOL )

#define GetMultiString1(sec,name,idx)				GetMulti1( sec, name, idx,  LTX_READER::VALUE_TYPES::STRING )
#define GetMultiString2(sec,name,idx1, idx2)		GetMulti2( sec, name, idx1, idx2, LTX_READER::VALUE_TYPES::STRING )
#define GetMultiString3(sec,name,idx1, idx2, idx3)	GetMulti3( sec, name, idx1, idx2, idx3, LTX_READER::VALUE_TYPES::STRING )


#endif//FILEREADER_H
