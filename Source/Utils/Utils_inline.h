// Console

#pragma once

#include "..\Defines.h"	// EMPTY_VEC3

// Print Info in Debug and Release
void cMsgINFO( LPCSTR format, ... ) {

	char buffer[ 1024 ];
	{
		va_list arg;
		va_start( arg, format );
		vsnprintf( buffer, sizeof( buffer ), format, arg );
		va_end( arg );
	}
	fprintf( stdout, "%s\n\n", buffer );

}

// Print Info in Debug
void cMsgDBG( LPCSTR format, ... ) {

#if defined( DEBUG )
	{
		va_list arg;
		fprintf( stdout, "--------[cDebug]: " );
		va_start( arg, format );
		vfprintf( stdout, format, arg );
		va_end( arg );
	}
	fprintf( stdout, "\n\n" );
#endif

}

// Print Info in Debug and Release
void cMsgCRT( LPCSTR format, ... ) {

	char buffer[ 1024 ];
	{
		va_list arg;
		va_start( arg, format );
		vsnprintf( buffer, sizeof( buffer ), format, arg );
		va_end( arg );
	}
	fprintf( stdout, "-----------------[!cCRITICAL!]: %s\n\n", buffer );
#if !defined( DEBUG )
	if ( Leadwerks::Window::GetCurrent() )
		MessageBox( Leadwerks::Window::GetCurrent()->hwnd, buffer, "Error", MB_ICONWARNING | MB_OK | MB_APPLMODAL );
	else
		MessageBox( NULL, buffer, "Error", MB_ICONWARNING | MB_OK | MB_APPLMODAL );
#endif

}


//****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\*/
/*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*///


// IO funcs


// Check if a file exists
bool sFileExists( LPCSTR FileName ) {

	if ( FILE *File = fopen( FileName, "r") ) {
		fclose( File );
		return true;
	}
	return false;

}

// Check if a file exists
bool FileExists( std::string FileName ) { return sFileExists( FileName.c_str() ); };

// Get a list of file located in PATH with EXT i.e ".ogg"
std::vector< std::string > ListFiles( std::string strPath, LPCSTR ExtFile ) {

	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fdata;
	std::string strFullPath;
	std::stack< std::string > stkFolders;
	stkFolders.push( strPath );
	std::vector< std::string > vecFiles;
	vecFiles.clear();
	
	while ( !stkFolders.empty() ) {
		strPath = stkFolders.top();
		strFullPath = strPath + "\\*";
		stkFolders.pop();
		
		hFind = FindFirstFile( ( strFullPath ).c_str(), &fdata );
		if ( hFind != INVALID_HANDLE_VALUE ) {
			do {
			//	cMessage( "%s", fdata.cFileName );
				if ( ( strcmp( fdata.cFileName, "." ) != 0 ) && ( strcmp( fdata.cFileName, ".." ) != 0 ) ) {
					if ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
						stkFolders.push( strPath + "/" + fdata.cFileName );
					} else {
						std::string FilePath = std::string( fdata.cFileName );
						if ( FilePath.substr( FilePath.find_last_of( '.' ) ) == ExtFile )
							vecFiles.push_back( strPath + "/" + fdata.cFileName );
					}
				}
			} while ( FindNextFile( hFind, &fdata ) != 0 );
		}
		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}
	return vecFiles;

}


//****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\*/
/*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*///

// String manipulation

// Remove extension from FILENAME
std::string RemoveExtension( std::string FileName ) {

	return FileName.substr( 0, FileName.find_last_of( '.' ) );

}

// Get EXTENSION from FilePath
std::string GetExtFromFilePath( std::string FilePath ) {

	return FilePath.substr( FilePath.find_last_of( '.') );

}

// Get NAME from FilePath
// if bExt, file extension is left, else removed
std::string GetNameFromFilePath( std::string FilePath, bool bExt ) {

	std::string FileName = FilePath.substr( FilePath.find_last_of( "\\/" ) + 1 );
	return ( bExt ) ? FileName : RemoveExtension( FileName );

}

// Get PATH from filePath
// If Slash is true another char is left in the string
std::string GetPathFromFilePath( std::string FilePath, bool Slash ) {

	return FilePath.substr( 0, FilePath.find_last_of( "\\/" ) + ( ( Slash ) ? 1 : 0 ) );

}

// Replace all '\\' and '/' with a dot
std::string String_Transform( std::string FilePath ) {

	std::string Result = FilePath;
	std::replace( Result.begin(), Result.end(), '\\', '.' );	// replace all 'x' to 'y'
	std::replace( Result.begin(), Result.end(), '/', '.' );		// replace all 'x' to 'y'
	return Result;

}

// Replace all dots with '\\'
std::string String_TransformToStd( std::string FilePath ) {

	std::string Result = FilePath;
	std::replace( Result.begin(), Result.end(), '.', '\\' );	// replace all 'x' to 'y'
	return Result;

}

// Trim a string from left side
// Clean every space after first alpha
void String_TrimL( std::string &String ) {

	size_t size = String.size();
	if ( !size ) return;
	for( size_t i = 0; i < size; i++ ) {
		if ( isspace( String[ i ] ) ) { String.erase( i, 1 ); size--; }
	}

}

// Trim a string from right side
// Clean every space before last alpha
void String_TrimR( std::string &String ) {

	size_t size = String.size();
	if ( !size ) return;
	for( size_t i = size; i > 0; i-- ) {
		if ( isspace( String[ i ] ) ) { String.erase( i, 1 ); i++; }
	}

}

// Trim a string by left and by right
std::string &String_Trim( std::string &String ) {

	String_TrimL( String );
	String_TrimR( String );
	return String;

}

// If semicolon is found, cuts string till there, semicolon apart
void String_CleanComments( std::string &String ) {

	if ( String.empty() ) return;
	for ( size_t i = 0; i < String.size(); i++ ) {
		if ( String[ i ] == ';' ) {
			String.erase( i );
			return;
		}
	}

}

// Return true if string contains al last one alpha char
bool String_ContainsAlpha( const std::string &String ) {

	const char *sString = String.data();
	for ( size_t i = 0; i < String.size(); i++ ) {
		if ( std::isalpha( sString[ i ] ) ) {
			return true;
		}
	}
	return false;

}

// Return true if string contains al last one digit char ( 0123456789 )
bool String_ContainsDigit( const std::string &String ) {

	const char *sString = String.data();
	for ( size_t i = 0; i < String.size(); i++ ) {
		if ( std::isdigit( sString[ i ] ) ) {
			return true;
		}
	}
	return false;

}


// clean comments and check if str contains chars or digits
bool String_IsValid( std::string &String ) {

	String_CleanComments( String );
	if ( String.empty() || ( !String_ContainsAlpha( String ) && !String_ContainsDigit( String ) ) ) return false;
	return true;

}

// If first char is not capital, then make it capital
void String_CapitalizeFirstChar( std::string &String ) {

	String[ 0 ] = std::toupper( String[ 0 ] );

}

// Read string correctly if format is Key = Value ; comment( optional )
// Returns a struct with key and value as members if read success
// Or return a struck with only key, that can by part of a list
KeyValue_t GetKeyValue( std::string Line ) {
	KeyValue_t Result;
	Result.IsOK = false;
	Result.iType = TYPE_KEYVALUE_NONE;
	Result.Key = Result.Value = "";

	if ( !String_IsValid( Line ) ) return Result;

	// IF KEY VALUE LINE
	size_t EqualSign = 0;
	for ( size_t i = 0; i < Line.size(); i++ )
		if ( Line[ i ]  == '=' ) { EqualSign = i; break; }
		
	if ( EqualSign > 0 ) { // Key Value Pair
		std::string sKey = String_Trim( Line.substr( 0, EqualSign ) );
		std::string sValue = Line.substr( EqualSign + 1 , std::string::npos );
		if ( sValue.size() ) sValue = String_Trim( sValue.substr( 0, sValue.find(';') ) );
		if ( !sKey.empty() ) {
			Result.Key = sKey;
			Result.Value = sValue;
			Result.iType = TYPE_KEYVALUE_KEYVALUE;
			Result.IsOK = true;
			return Result;
		}
	}

	if ( Result.iType == TYPE_KEYVALUE_NONE ) {

		// IF KEY LIST
		std::string sKey = String_Trim( Line.substr( 0, Line.find(';') ) );
		if ( !sKey.empty() ) {
			Result.Key = sKey;
			Result.Value = "";
			Result.iType = TYPE_KEYVALUE_LISTKEY;
			Result.IsOK = true;
			Result;
		}
	}

	return Result;
}


//****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\*/
/*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*///

// Conversions

// Create a 2D vector from string
Leadwerks::Vec2 StrToVec2( std::string string ) {
	float x, y;
	sscanf( string.c_str(), "%f,%f", &x, &y );
	return { x, y };
}

// Create a 2D vector from string
Leadwerks::Vec3 StrToVec3( std::string string ) {
	float x, y, z;
	sscanf( string.c_str(), "%f,%f,%f", &x, &y, &z );
	return { x, y, z };
}

// Return an std vector with 1 = x, 2 = y, 3 = z;
std::vector < float > LeadwerksV3ToSTDVec( Leadwerks::Vec3 v ) {
	return { v.x, v.y, v.z };
}

//****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\*/
/*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*///

// Math

// Return 1 for numbers more than zero otherwise -1
char Sign( float n ) {	return ( n > -1.0 ) ? 1:-1; }

// Return true if abs of a is really near to b
bool Similar( float a, float b, float c ) { return abs( a - b ) < c; }

// Return true if abs of a is really near to Zero
bool SimilarZero( float a, float cmp ) { return abs( a ) < cmp; }

// Return an average of values contained in vector
// only accepts float vectors!!
float fAverage( std::vector< float > Values ) { return std::accumulate( Values.begin(), Values.end(), 0.0 ) / Values.size(); }

//return clamped vector between min and max for x, y, and z
Leadwerks::Vec3 Vec3Clamp ( Leadwerks::Vec3 v, float min, float max ) {
	return { L_CLAMP( v.x, min, max ), L_CLAMP( v.y, min, max ), L_CLAMP( v.z, min, max ) };
}

// return clamped vector between corrisponding min e max for x, y and z
Leadwerks::Vec3 Vec3Clamp ( Leadwerks::Vec3 v, float minx, float maxx, float miny, float maxy, float minz, float maxz ) {
	return { L_CLAMP( v.x, minx, maxx ), L_CLAMP( v.y, miny, maxy ), L_CLAMP( v.z, minz, maxz ) };
}

// Return an average of vector contained in vector
Leadwerks::Vec3 Vec3Average( std::vector < Leadwerks::Vec3 > Values ) {
	float X = 0.0f, Y = 0.0f, Z = 0.0f;
	for ( auto v : Values ) { X += v.x; Y += v.y; Z += v.z; }
	return Leadwerks::Vec3( X, Y, Z ) / Values.size();
}

// Curve an entire vector to fVal with f divisions
Leadwerks::Vec3 Vec3Curve( float f, Leadwerks::Vec3 v, float d ) {
	return { L_CURVE( f, v.x, d ), L_CURVE( f, v.y, d ), L_CURVE( f, v.z, d ) };
}

// Curve an entire vector to fVal with x, y and z custom divisions
Leadwerks::Vec3 Vec3Curve( float f, Leadwerks::Vec3 v, float dX, float dY, float dZ ) {
	return { L_CURVE( f, v.x, dX ), L_CURVE( f, v.y, dY ), L_CURVE( f, v.z, dZ ) };
}

// Curve an entire vector from v2 to v1 with f divisions
Leadwerks::Vec3 Vec3Curve( Leadwerks::Vec3 v1, Leadwerks::Vec3 v2, float d ) {
	return { L_CURVE( v1.x, v2.x, d ), L_CURVE( v1.y, v2.y, d ), L_CURVE( v1.z, v2.z, d ) };
}

// Curve an entire vector from v2 to v1 with x, y and z custom divisions
Leadwerks::Vec3 Vec3Curve( Leadwerks::Vec3 v1, Leadwerks::Vec3 v2, float dX, float dY, float dZ ) {
	return { L_CURVE( v1.x, v2.x, dX ), L_CURVE( v1.y, v2.y, dY ), L_CURVE( v1.z, v2.z, dZ ) };
}

// Interpolates between the vectors a and b by the interpolant t
Leadwerks::Vec3 Vec3Lerp( Leadwerks::Vec3 a, Leadwerks::Vec3 b, float t ) {
	t = L_CLAMP( t, 0.0, 1.0 );
	return { L_LERP( a.x, b.x, t ), L_LERP( a.y, b.y, t ), L_LERP( a.z, b.z, t ) };
}

// Interpolates between the vectors a and b by the interpolant tX, tY and tZ
Leadwerks::Vec3 Vec3LerpXYZ( Leadwerks::Vec3 a, Leadwerks::Vec3 b, float tX, float tY, float tZ ) {
	tX = L_CLAMP( tX, 0.0, 1.0 ); tY = L_CLAMP( tY, 0.0, 1.0 ); tZ = L_CLAMP( tZ, 0.0, 1.0 );
	return { L_LERP( a.x, b.x, tX ), L_LERP( a.y, b.y, tY ), L_LERP( a.z, b.z, tZ ) };
}

// Interpolates between the vectors a and b by the interpolant t
// Slerp is useful for rotation, mostly
Leadwerks::Vec3 Vec3SLerp( Leadwerks::Vec3 a, Leadwerks::Vec3 b, float t ) {
	float dot = a.Dot( b );
	dot = Leadwerks::Math::Clamp( dot, -1.0f, 1.0f );
	float theta = Leadwerks::Math::ACos( dot ) * t;
	Leadwerks::Vec3 RelativeVec = b - a*dot;
	RelativeVec.Normalize();
	return ( ( a * Leadwerks::Math::Cos( theta ) ) + ( RelativeVec * Leadwerks::Math::Sin( theta ) ) );
}

// Interpolates between max three vectors by the interpolant t
Leadwerks::Vec3 Vec3MultiLerp( std::vector < Leadwerks::Vec3 > &vVectors, float t ) {

	Leadwerks::Vec3 vResult = EMPTY_VEC3;

	switch( vVectors.size() ) {

			// no variation so return EMPTY_VEC3
		case 0 : break;

			// Only one rotation,set that
		case 1 : {
			vResult = vVectors[ 0 ];
			break;
		}
			// Interpolation between two vectors of coordinates
		case 2 : {
			vResult = Vec3Lerp( vVectors[ 0 ], vVectors[ 1 ], t ); 
			break;
		}
			// in every case interpolate the last three vectors of coordinates
		case 3 : default: {
			Leadwerks::Vec3 vResult1 = Vec3Lerp( vVectors[ 0 ], vVectors[ 1 ], t );
			Leadwerks::Vec3 vResult2 = Vec3Lerp( vVectors[ 1 ], vVectors[ 2 ], t );
			vResult = Vec3Lerp( vResult1, vResult2, t );
			break;
		}

		break;
	}

	return vResult;
}

// Interpolates between max three vectors by the interpolant t
// Is useful for rotation, but is very 
Leadwerks::Vec3 Vec3MultiSLerp( std::vector < Leadwerks::Vec3 > &vVectors, float t ) {

	Leadwerks::Vec3 vResult = EMPTY_VEC3;

	switch( vVectors.size() ) {

			// no variation so return EMPTY_VEC3
		case 0 : break;

			// Only one rotation, set that
		case 1 : {
			vResult = vVectors[ 0 ];
			break;
		}
			// Interpolation between two vectors of coordinates
		case 2 : {
			vResult = Vec3SLerp( vVectors[ 0 ], vVectors[ 1 ], t ); 
			break;
		}
			// in every case interpolate the last three vectors of coordinates
		case 3 : default: {
			Leadwerks::Vec3 vResult1 = Vec3SLerp( vVectors[ 0 ], vVectors[ 1 ], t );
			Leadwerks::Vec3 vResult2 = Vec3SLerp( vVectors[ 1 ], vVectors[ 2 ], t );
			vResult = Vec3SLerp( vResult1, vResult2, t );
			break;
		}

		break;
	}

	return vResult;
}


//****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\*/
/*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*///

// Assignaments

// Assign x and y to x and y menbers of Vec2
void AssignVec2To( Leadwerks::Vec2 &Vec2, float x, float y ) {
	Vec2.x = x; Vec2.y = y;
}

// Assign x and y to x and y menbers of Vec2
void AssignVec2To( Leadwerks::Vec2 *Vec2, float x, float y ) {
	Vec2->x = x; Vec2->y = y;
}

// Assign x, y and z to x, y and z menbers of Vec3
void AssignVec3To( Leadwerks::Vec3 &Vec3, float x, float y, float z ) {
	Vec3.x = x; Vec3.y = y; Vec3.z = z;
}

// Assign x, y and z to x, y and z menbers of Vec3
void AssignVec3To( Leadwerks::Vec3 *Vec3, float x, float y, float z ) {
	Vec3->x = x; Vec3->y = y; Vec3->z = z;
}

// Usefull for multi assignaments


// Assign v1 and v2 to V1 and V2
void SetInt2( int &V1, int &V2, int v1, int v2 ) { V1 = v1; V2 = v2; }

// Assign v1, v2 and v3 to V1, V2 and V3
void SetInt3( int &V1, int &V2, int &V3, int v1, int v2, int v3 ) { V1 = v1; V2 = v2; V3 = v3; }

// Assign v1 and v2 to V1 and V2
void SetBool2( bool &V1, bool &V2, bool v1, bool v2 ) { V1 = v1; V2 = v2; }

// Assign v1, v2 and v3 to V1, V2 and V3
void SetBool3( bool &V1, bool &V2, bool &V3, bool v1, bool v2, bool v3 ) { V1 = v1; V2 = v2; V3 = v3; }

// Assign v1 and v2 to V1 and V2
void SetFloat2( float &V1, float &V2, float v1, float v2 ) { V1 = v1; V2 = v2; }

// Assign v1, v2 and v3 to V1, V2 and V3
void SetFloat3( float &V1, float &V2, float &V3, float v1, float v2, float v3 ) { V1 = v1; V2 = v2; V3 = v3; }

// Assign v1 and v2 to V1 and V2
void SetString2( std::string &V1, std::string &V2, std::string v1, std::string v2 ) { V1 = v1; V2 = v2; }

// Assign v1, v2 and v3 to V1, V2 and V3
void SetString3( std::string &V1, std::string &V2, std::string &V3, std::string v1, std::string v2, std::string v3 ) { V1 = v1; V2 = v2; V3 = v3; }

