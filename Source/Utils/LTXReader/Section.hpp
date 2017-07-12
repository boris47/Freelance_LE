#ifndef SECTION_H
#define SECTION_H
#pragma once


class cSection {
	private:
		std::vector< cLineValue* >				vSection;
		typedef std::vector< cLineValue* >		SectionType;
		std::vector< cLineValue* >::iterator	vSectionI = vSection.begin();
		std::string	sName						= "";
		cValue *pDefaultCValue					= new cValue();

		bool bIsOK = false;
	
	public:
		
		cSection( std::string Name, cSection* Inherited = NULL ) {
			sName = Name;
			if ( Inherited ) vSection = Inherited->GetData();
			bIsOK = true;
		}
		~cSection( void ) {
			for ( auto pSection : vSection )  delete pSection;
			vSection.clear();
			SAFE_DELETE( pDefaultCValue );
		}
		inline	bool				IsOK()				{ return bIsOK == false; }
		
		inline	SectionType			GetData()			{ return vSection; }
		inline	size_t				Lines( void )		{ return vSection.size(); }
		inline	std::string			Name( void ) const	{ return sName; }

		inline	void				Add( cLineValue *LineValue );
		inline	cLineValue *		GetLineValue( std::string Key );
		inline	bool				HasKey( std::string Key );
		inline	int					KeyType( std::string Key );
		


		inline	std::string			GetRawValue ( std::string Key, std::string	Default = std::string( "" ) );

		inline	int					GetInt		( std::string Key, int			Default = -1 );
		inline	bool				GetBool		( std::string Key, bool			Default = false);
		inline	float				GetFloat	( std::string Key, float		Default = -1.0 );
		inline	std::string			GetString	( std::string Key, std::string	Default = std::string( "" ) );
		inline	size_t				GetMultiSize( std::string Key );
		inline	cValue&				GetMultiValue( std::string Key, size_t Index, int Type );
		inline	Leadwerks::Vec2		GetVec2		( std::string Key );
		inline	Leadwerks::Vec3		GetVec3		( std::string Key );
		inline	Leadwerks::Vec4		GetVec4		( std::string Key );


		inline	bool				bGetInt		( std::string Key, int &Out,			int			Default = -1 );
		inline	bool				bGetBool	( std::string Key, bool &Out,			bool		Default = false );
		inline	bool				bGetFloat	( std::string Key, float &Out,			float		Default = -1.0 );
		inline	bool				bGetString	( std::string Key, std::string &Out,	std::string	Default = std::string( "" ) );
		inline	bool				bGetMultiValue( std::string Key, cValue *&Out, size_t Index, int Type );
		inline	bool				bGetVec2	( std::string Key, Leadwerks::Vec2 &Out, Leadwerks::Vec2 Default = { 0.0, 0.0 } );
		inline	bool				bGetVec3	( std::string Key, Leadwerks::Vec3 &Out, Leadwerks::Vec3 Default = { 0.0, 0.0, 0.0 } );
		inline	bool				bGetVec4	( std::string Key, Leadwerks::Vec4 &Out, Leadwerks::Vec4 Default = { 0.0, 0.0, 0.0, 0.0 } );


		inline	void				SetInt		( std::string Key, int Value );
		inline	void				SetBool		( std::string Key, bool Value );
		inline	void				SetFloat	( std::string Key, float Value );
		inline	void				SetString	( std::string Key, std::string Value );
		inline	void				SetValue	( std::string Key, cValue *Value );
		inline	void				SetMultiValue( std::string Key, std::vector < cValue * > vValues );
		inline	void				SetVec2		( std::string Key, Leadwerks::Vec2 &Vec );
		inline	void				SetVec3		( std::string Key, Leadwerks::Vec3 &Vec );
		
		inline void					Print( void );
	
};

#endif//SECTION_H

#include "Section_Inline.hpp"