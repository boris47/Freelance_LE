#ifndef VALUE_H
#define VALUE_H
#pragma once

#include <string>

#include "Defines.hpp"

class cValue {
	private:
		int iType = LTX_READER::VALUE_TYPES::NONE;
		int iInt;
		bool bBool;
		float fFloat;
		std::string sString;
	
	public:
		int Type( void )			{ return iType; };
		cValue()					: iInt(-1), bBool(false), fFloat(-1.0f), sString("") { iType = LTX_READER::VALUE_TYPES::NONE; };
		cValue( int iVal )			{ iInt = iVal;		iType = LTX_READER::VALUE_TYPES::INT;		};
		cValue( bool bVal )			{ bBool = bVal;		iType = LTX_READER::VALUE_TYPES::BOOL;		};
		cValue( float fVal )		{ fFloat = fVal;	iType = LTX_READER::VALUE_TYPES::FLOAT;		};
		cValue( std::string sVal )	{ sString = sVal;	iType = LTX_READER::VALUE_TYPES::STRING;	};
		
		// DANGEROUS
		void SetType( int Type ) { iType = Type; } 

		int ToInt( void );
		bool ToBool( void );
		float ToFloat( void );
		std::string ToString( void );

		operator int()			{ return ToInt(); };
		operator bool()			{ return ToBool(); };
		operator float()		{ return ToFloat(); };
		operator std::string()	{ return ToString(); };
		
		void Print( void );
	
};

#endif//VALUE_H