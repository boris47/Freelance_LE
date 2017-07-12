
#include "XMLReader.h"

pugi::xml_document* CXMLReader::LoadFile( std::string FileName ) {
	
	if ( !FileExists( FileName ) ) return 0;
	
	pugi::xml_document xmlDoc;
	/*
	if ( xmlDoc.load_file( FileName.c_str() ) != pugi::xml_parse_result.status.status_ok ) return 0;
	
	mXMLDocs[ FileName ] = xmlDoc;
	*/
	return &pugi::xml_document();
}

pugi::xml_document* CXMLReader::GetFile( std::string FileName ) {
	if ( !FileExists( FileName ) ) return 0;
	
	pugi::xml_document xmlDoc;
	
//	try { xmlDoc( mXMLDocs.at( FileName ) ); } catch ( ... ) {}
	
	return &pugi::xml_document();
}

pugi::xml_node* CXMLReader::HasChild( std::string FileName ) {
	return &pugi::xml_node();
}

pugi::xml_node* CXMLReader::HasChild( pugi::xml_node& Node ) {
	return &pugi::xml_node();
}

int GetInt( std::string FileName, std::string RootNode, std::string Attribute, int Default = -1 ) {
	return 0;
}

int GetInt( std::string FileName, std::string RootNode, std::string ResNode, std::string Attribute, int Default = -1 ) {
	return 0;
}

int GetInt( pugi::xml_node& Node, std::string Attribute, int Default = -1 ) {
	return 0;
}


bool GetBool( std::string Child, std::string Attribute, bool Default = false ) {
	return true;
}

bool GetBool( std::string FileName, std::string RootNode, std::string ResNode, std::string Attribute, bool Default = false ) {
	return true;
}

bool GetBool( pugi::xml_node& Node, std::string Attribute, bool Default = false ) {
	return true;
}


double GetDouble( std::string Child, std::string Attribute, double Default = -1.0 ) {
	return 1.0;
}

double GetDouble( std::string FileName, std::string RootNode, std::string ResNode, std::string Attribute, double Default = -1.0 ) {
	return 1.0;
}

double GetDouble( pugi::xml_node& Node, std::string Attribute, double Default = -1.0 ) {
	return 1.0;
}


std::string GetString( std::string Child, std::string Attribute, std::string Default = std::string("") ) {
	return std::string( "" );
}

std::string GetString( std::string FileName, std::string RootNode, std::string ResNode, std::string Attribute, std::string Default = std::string("") ) {
	return std::string( "" );
}

std::string GetString( pugi::xml_node& Node, std::string Attribute, std::string Default = std::string("") ) {
	return std::string( "" );
}