
#pragma once


bool cINI_Section::AddLine( std::string Line ) {

	KeyValue_t t = GetKeyValue( Line );
	if ( !t.IsOK ) return false;

	mSectionMap[ Trim( t.Key ) ] = Trim( t.Value );
	return true;

}

bool cINI_Section::AddKeyValue( std::string Key, std::string Value ) {

	if ( !Key.length() ) return false;
	mSectionMap[ Trim( Key ) ] = Trim( Value );
	return true;

}

cINI_Section::SectionMap cINI_Section::GetData( void ) {

	return SectionMap( mSectionMap );

}

bool cINI_Section::HasKey( std::string Key ) {

	if ( mSectionMap.empty() ) return false;
	try { mSectionMap.at( Key ); }
	catch ( ... ) { return false; }
	return true;

}

bool cINI_Section::GetValue( std::string Key, std::string &Value ) {

	if ( mSectionMap.empty() ) return false;
	try { Value = mSectionMap.at( Key ); }
	catch ( ... ) { return false; }
	return true;

}

std::string cINI_Section::GetRawLine( std::string Key, std::string	Default ) {

	std::string Value;
	return GetValue( Key, Value ) ? Key + " = " + Value : Default;

}



//// CONVERSION
template< typename T >
inline bool ConvertSTR( const std::string& str, T &ret ) {
	
	if ( !str.length() ) return false;
	std::istringstream iss( str );
	if ( !( iss >> std::ws >> ret >> std::ws ) || ( !iss.eof() ) ) return false;
	return true;

}


int cINI_Section::GetInt( std::string Key, int Default ) {

	std::string Value;
	int ReturnVal;
	if ( GetValue( Key, Value ) && ( ConvertSTR< int >( Value, ReturnVal  ) ) )
		return ReturnVal;
	return Default;

}
bool cINI_Section::bGetInt( std::string Key, int &Out, int Default ) {
	
	std::string Value;
	if ( GetValue( Key, Value ) && ( ConvertSTR< int >( Value, Out  ) ) ) return true;
	Out = Default;
	return false;

}


bool cINI_Section::GetBool( std::string Key, bool Default ) {

	std::string Value;
	if ( GetValue( Key, Value ) )
		return ( ( Value == "true" ) || ( Value == "1" ) ) ? true : false;
	return Default;

}
bool cINI_Section::bGetBool( std::string Key, bool &Out, bool Default ) {
	
	std::string Value;
	if ( GetValue( Key, Value ) ) {
		Out = ( ( Value == "true" ) || ( Value == "1" ) ) ? true : false;
		return true;
	}

	Out = Default;
	return false;
}


float cINI_Section::GetFloat( std::string Key, float Default ) {

	std::string Value;
	float ReturnVal;
	if ( GetValue( Key, Value ) && ( ConvertSTR< float >( Value, ReturnVal  ) ) )
		return ReturnVal;
	return Default;

}
bool cINI_Section::bGetFloat( std::string Key, float &Out, float Default ) {
	
	std::string Value;
	if ( GetValue( Key, Value ) && ( ConvertSTR< float >( Value, Out  ) ) ) return true;
	Out = Default;
	return false;

}


std::string cINI_Section::GetString( std::string Key, std::string Default ) {

	std::string Value;
	if ( GetValue( Key, Value ) ) return Value;
	return Default;

}
bool cINI_Section::bGetString( std::string Key, std::string &Out, std::string Default ) {

	std::string Value;
	if ( GetValue( Key, Value ) ) {
		Out = Value;
		return true;
	}
	Out = Default;
	return false;

}

bool cINI_Section::IsMultivalue( std::string Key ) {
	
	std::string Value;
	if ( GetValue( Key, Value ) ) {
		return ( Value.find( "," ) != std::string::npos );
	}
	return false;

}

size_t cINI_Section::GetMultiSize( std::string Key ) {

	if ( !IsMultivalue( Key ) ) return 0;

	std::string Value;
	GetValue( Key, Value );

	size_t n = 0;
	for ( size_t i = 0; i < Value.length(); i++ ) if ( Value[ i ] == ',' ) n++;
	return n;

}



template < typename T >
bool cINI_Section::GetMultiValue( std::string Key, T &Out, size_t Index, int Type ) {

	if ( !IsMultivalue( Key ) ) return false;

	std::string ValueLine;
	GetValue( Key, ValueLine );

	size_t n = ValueLine.find( "," );
	std::string Value = "";
	switch ( Index ) {

		case 1: Value = ValueLine.substr( 0, n );
		case 2: Value = ValueLine.substr( n, ValueLine.find( ",", n ) );
		case 4: {

			for ( size_t i = n; i < ValueLine.length(); i++ ) {

				if ( ValueLine[ i ] == ',' ) {
					if ( i == Index ) {
						Value = ValueLine.substr( n, i );
						break;
					}
					n = i;
				}
			}
		}
	}

	if ( !Value.length() ) return false;

	switch ( Type ) {
		case INI_SYSTEM::VALUE_TYPE::INT: return ConvertSTR < int > ( Value, Out );
		case INI_SYSTEM::VALUE_TYPE::BOOL: {
			Out = ( Value == "true" ) || ( Value == "1" ) ) ? true : false;
			return true;
		}
		case INI_SYSTEM::VALUE_TYPE::FLOAT: return ConvertSTR < float > ( Value, Out );
		case INI_SYSTEM::VALUE_TYPE::STRING: { 
			Out = Value;
			return true;
		}
	}
}

