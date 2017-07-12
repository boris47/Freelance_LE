
#ifndef ELEMENT_H
#define ELEMENT_H
#pragma once


class cFileElement {

	private:
		cSection *	pSection	= NULL;
		cList *		pList		= NULL;
		char		iType		= LTX_READER::FILE_ELEMENT::NONE;

	public:
		cFileElement( char Type ) : iType( Type ) {}
		~cFileElement() { SAFE_DELETE( pSection ); SAFE_DELETE( pList ); }

		inline	char		GetType( void ) { return iType; }

		inline	cSection *	GetSection( void );
		inline	void		SetSection( cSection *Section );

		inline	cList *		GetList( void );
		inline	void		SetList( cList *List );

};


cSection *cFileElement::GetSection( void ) {

	if ( iType != LTX_READER::FILE_ELEMENT::SECTION ) {
		cMsgDBG( "cFileElement::GetSection:Trying to retrieve a non section element" );
		return NULL;
	}

	if ( !pSection ) {
		cMsgCRT( "cFileElement::GetSection:Trying to retrieve a section element, but actual value is NULL " );
		return NULL;
	}

	return pSection;
}


void cFileElement::SetSection( cSection *Section ) {
	if ( Section && ( iType == LTX_READER::FILE_ELEMENT::SECTION ) ) {
		pSection = Section;
		return;
	}
	cMsgCRT( "cFileElement::SetSection:Trying to set a non section value" );
}


cList *cFileElement::GetList( void ) {

	if ( iType != LTX_READER::FILE_ELEMENT::LIST ) {
		cMsgDBG( "cFileElement::GetList:Trying to retrieve a non list element" );
		return NULL;
	}

	if ( !pList ) {
		cMsgCRT( "cFileElement::GetList:Trying to retrieve a list element, but actual value is NULL " );
		return NULL;
	}

	return pList;
}


void cFileElement::SetList( cList *List ) {
	if ( List && ( iType == LTX_READER::FILE_ELEMENT::LIST ) ) {
		pList = List;
		return;
	}
	cMsgCRT( "cFileElement::SetList:Trying to set a non list value" );
}


#endif//ELEMENT_H