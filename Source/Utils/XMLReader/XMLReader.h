
#pragma once

#include "PugiXML\\pugixml.hpp"
#include "..\\Utils.h"

class CXMLReader {
	private:
		std::map < std::string, pugi::xml_document > mXMLDocs;
	
	public:
		pugi::xml_document* LoadFile( std::string FileName );
		pugi::xml_document* GetFile( std::string FileName );
		
		pugi::xml_node* HasChild( std::string FileName );
		pugi::xml_node* HasChild( pugi::xml_node& Node );
		
		int GetInt( std::string FileName, std::string RootNode, std::string Attribute, int Default = -1 );
		int GetInt( std::string FileName, std::string RootNode, std::string ResNode, std::string Attribute, int Default = -1 );
		int GetInt( pugi::xml_node& Node, std::string Attribute, int Default = -1 );
		
		bool GetBool( std::string Child, std::string Attribute, bool Default = false );
		bool GetBool( std::string FileName, std::string RootNode, std::string ResNode, std::string Attribute, bool Default = false );
		bool GetBool( pugi::xml_node& Node, std::string Attribute, bool Default = false );
		
		double GetDouble( std::string Child, std::string Attribute, double Default = -1.0 );
		double GetDouble( std::string FileName, std::string RootNode, std::string ResNode, std::string Attribute, double Default = -1.0 );
		double GetDouble( pugi::xml_node& Node, std::string Attribute, double Default = -1.0 );
		
		std::string GetString( std::string Child, std::string Attribute, std::string Default = std::string("") );
		std::string GetString( std::string FileName, std::string RootNode, std::string ResNode, std::string Attribute, std::string Default = std::string("") );
		std::string GetString( pugi::xml_node& Node, std::string Attribute, std::string Default = std::string("") );
		
	
};