#ifndef LINEVALUE_H
#define LINEVALUE_H
#pragma once

#include "MultiValue.hpp"
#include "Value.hpp"

class cLineValue {
	private:
		int iType 				= LTX_READER::VALUE_TYPES::NONE;
		cValue *Value 			= nullptr;
		cMultiValue *MultiValue = nullptr;
		std::string sKey 		= "";
		std::string sRawValue 	= "";
		bool bIsOK 				= false;
	
	public:
		cLineValue( std::string Key, int Type );
		cLineValue( std::string Key, std::string Line );
		~cLineValue( void );
		inline	bool IsOK() 				  { return bIsOK; }
		
		inline 	bool IsKey( std::string Key ) { return ( sKey == Key ); }

		// Can be NONE, SINGLE, MULTI, KEYONLY
		inline 	int Type( void ) 			  { return iType; };
		inline 	std::string Key() 		const { return sKey; }
		inline 	std::string RawValue() 	const { return sRawValue; }
		inline 	void Clear( void );
		
		inline 	cLineValue *Set( cValue *_Value )			{ Value = _Value; return this; }
		inline 	cLineValue *Set( cMultiValue *_MultiValue )	{ MultiValue = _MultiValue; return this;	}
		
		inline 	cValue		*GetValue( void )		{ return Value; }
		inline 	cMultiValue	*GetMultiValue( void )	{ return MultiValue;	}
		
		inline 	void Print( void ) {
			if ( Value ) { Value->Print(); return; };
			if ( MultiValue ) { MultiValue->Print(); return; };
		}
		
};

#endif//LINEVALUE_H