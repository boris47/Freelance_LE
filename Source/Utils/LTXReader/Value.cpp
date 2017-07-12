
#include "Value.hpp"
#include "Defines.hpp"
#include "..\Utils.h"

void PrintValueType( int Value ) {
	switch( Value ) {
		case LTX_READER::VALUE_TYPES::INT		: { cMsgCRT( "Value is of type INT!" );		return; }
		case LTX_READER::VALUE_TYPES::BOOL		: { cMsgCRT( "Value is of type BOOL!" );	return; }
		case LTX_READER::VALUE_TYPES::FLOAT		: { cMsgCRT( "Value is of type FLOAT!" );	return; }
		case LTX_READER::VALUE_TYPES::STRING	: { cMsgCRT( "Value is of type STRING!" );	return; }
		case LTX_READER::VALUE_TYPES::NONE		: { cMsgCRT( "Value is of type NONE!" );	return; }
	}
	cMsgCRT( "Value has not type !!!" );
}

int cValue::ToInt( void ) {
	switch( iType ) {
		case LTX_READER::VALUE_TYPES::INT: return iInt;
		case LTX_READER::VALUE_TYPES::BOOL: return ( bBool ) ? 1 : 0;
		case LTX_READER::VALUE_TYPES::FLOAT: return int( fFloat );
	}
	cMsgCRT( "cValue::ToInt: request for a non castable to INT value!" );
	PrintValueType( iType );
	return 0;
}

bool cValue::ToBool( void ) {
	switch( iType ) {
		case LTX_READER::VALUE_TYPES::BOOL: return bBool;
		case LTX_READER::VALUE_TYPES::INT: return ( iInt > 0 ) ? true : false;
		case LTX_READER::VALUE_TYPES::FLOAT: return ( fFloat > 0 ) ? true : false;
	}
	cMsgCRT( "cValue::ToBool: request for a non castable to BOOL value!" );
	PrintValueType( iType );
	return false;
}

float cValue::ToFloat( void ) {
	switch( iType ) {
		case LTX_READER::VALUE_TYPES::FLOAT: return fFloat;
		case LTX_READER::VALUE_TYPES::BOOL: return ( bBool ) ? 1.0 : -1.0;
		case LTX_READER::VALUE_TYPES::INT: return float( iInt );
	}
	cMsgCRT( "cValue::ToFloat: request for a non castable to FLOAT value!" );
	PrintValueType( iType );
	return 0.0;
}

std::string cValue::ToString( void ) {
	switch( iType ) {
		case LTX_READER::VALUE_TYPES::INT: return std::to_string( iInt );
		case LTX_READER::VALUE_TYPES::BOOL: return ( bBool ) ? "true":"false";
		case LTX_READER::VALUE_TYPES::FLOAT: return std::to_string( fFloat );
		case LTX_READER::VALUE_TYPES::STRING:  return sString;
	}
	cMsgCRT( "cValue::ToString: request for a non castable to STRING value!" );
	PrintValueType( iType );
	return std::string( "" );
}

void cValue::Print( void ) {
	switch( iType ) {
		case LTX_READER::VALUE_TYPES::INT:    { cMsgDBG( "INT     |%i|", iInt ); break;}
		case LTX_READER::VALUE_TYPES::BOOL:   { cMsgDBG( "BOOL    |%i|", bBool ); break; }
		case LTX_READER::VALUE_TYPES::FLOAT:  { cMsgDBG( "FLOAT   |%f|", fFloat ); break; }
		case LTX_READER::VALUE_TYPES::STRING: { cMsgDBG( "STRING  |%s|", sString.c_str() ); break; }
	}
}
