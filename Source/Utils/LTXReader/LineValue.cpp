

#include "LineValue.hpp"


#include "Defines.hpp"	// namespace LTX_READER
#include "..\\Utils.h"	// for Trim


#include <sstream>		// std::istringstream

inline bool IsValidStringChar( char Char ) {

	if ( ( Char > 64 && Char < 91  ) || // A - Z
		 ( Char > 96 && Char < 123 ) || // a - z
		 ( Char == 58 ) 				// : ( Double dot )
		 )
		 return true;
	return false;
}

inline char ReturnType( std::string Line ) {
	bool IsStr = false, IsNumber = false, DotFound = false;
	for ( size_t i = 0; i < Line.size(); i++ ) {
		char Char = Line[ i ];
		if ( Char == 32 ) continue;								// skip parsing spaces
		if ( Char == 46 ) DotFound = true;						// (Dot)Useful for number determination
		if ( Char > 47 && Char < 58 && !IsStr ) {				// is number and not a str
			IsNumber = true;									// ok, now is a number
		}
		if ( IsValidStringChar( Char ) ) {						// is char [ A-Z ] or [ a-z ] or :
			IsStr = true; IsNumber = false;						// if was a number now is a string, never more a number
			break;
		}
	}
	
	if ( IsNumber ) {											// try understand if is a int or float type
		if ( DotFound ) return LTX_READER::VALUE_TYPES::FLOAT;	// because of dot is probably a float value
		else return LTX_READER::VALUE_TYPES::INT;				// No dot found so is probably an integer
	}
	if ( IsStr ) {												// try understand if is a string or boolean type
		if ( ( ( Line == "True" ) || ( Line == "true" ) ) ||
		( ( Line == "False" ) || ( Line == "false" ) ) ) {
			return LTX_READER::VALUE_TYPES::BOOL;
		}
		else return LTX_READER::VALUE_TYPES::STRING;
	}
	return LTX_READER::VALUE_TYPES::NONE;
}

inline cValue *RecognizeValue( std::string Line ) {	
	std::istringstream is( Line );
//	cMsgDBG( "FileReader:RecognizeValue:Riconoscimento di |%s|", Line.c_str() );
	switch( ReturnType( Line ) ) {
		case LTX_READER::VALUE_TYPES::NONE:	break;
		case LTX_READER::VALUE_TYPES::INT:	{ int iValue;	is >> iValue; return ( new cValue( iValue ) ); }
		case LTX_READER::VALUE_TYPES::FLOAT:{ float fValue; is >> fValue; return ( new cValue( fValue ) ); }
		case LTX_READER::VALUE_TYPES::BOOL:	{ 
			return ( ( Line == "True" ) || ( Line == "true" ) ) ? ( new cValue( true ) ) : ( new cValue( false ) );
		}
		case LTX_READER::VALUE_TYPES::STRING:{ return ( new cValue( Line ) ); }
	}
	return NULL;
}

inline std::vector< cValue * > RecognizeValues( std::string _Line ) {
	std::vector< cValue* > Values; Values.clear();
	std::string Line (_Line);
	size_t Start = 0;
//	cMsgDBG( "FileReader:RecognizeValues:Main Line |%s|", Line.c_str() );
	for ( size_t i = 0; i < Line.size(); i++ ) {
		if ( Line[ i ] == ',' ) {
			std::string Result = String_Trim( Line.substr( Start, i - Start ) );
			cValue *Value = RecognizeValue( Result );
			if ( Value ) Values.push_back( Value );
			Start = i + 1;
		}
	}
	cValue *Value = RecognizeValue( Line.substr( Start ) );	// last value is not followed by a colon
	if ( Value ) Values.push_back( Value );					// So we save the last part of string entirely
	return Values;
}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////



// Type must be LTX_READER::VALUE_TYPE:: + NONE or SINGLE or MULTI or KEYONLY
cLineValue::cLineValue( std::string Key, int Type ) { iType = Type; sKey = Key; }


cLineValue::cLineValue( std::string Key, std::string Line ) {

	sKey = Key;
	sRawValue = ( ( Line.size() > 0 ) ? Line : "" );

	if ( Line.size() == 0 ) { // supposed to be a list key only
		iType = LTX_READER::VALUE_TYPE::KEYONLY;
		bIsOK = true;
		return;
	}

	if ( Line.find( ',' ) != std::string::npos ) { // Supposing is a MultiVal string
		iType = LTX_READER::VALUE_TYPE::MULTI;
		std::vector< cValue * > vValues = RecognizeValues( Line );
		if ( !vValues.size() ) return;
		MultiValue = new cMultiValue( vValues );
		
	} else { // Single value
		iType = LTX_READER::VALUE_TYPE::SINGLE;
		cValue *Value = RecognizeValue( Line );
		if ( !Value ) return;
		this->Value = Value;
	}
	
	bIsOK = true;
}


cLineValue::~cLineValue( void ) {
	this->Clear();
}

void cLineValue::Clear( void ) {
	SAFE_DELETE( Value ); SAFE_DELETE( MultiValue );
}

