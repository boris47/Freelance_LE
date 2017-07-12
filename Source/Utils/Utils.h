
#pragma once

#include "Leadwerks.h"	// Leadwerks::Vec3
#include "MultiThread.h"

#include <stack>
#include <algorithm>
#include <numeric>
#include <windows.h>
#include <stdio.h>
#include <cstdio>
#include <random>
#include <string>
#include <vector>

#define TypeToStr(Type) #Type


// Console


// Print Info in Debug and Release
inline void cMsgINFO( LPCSTR format, ... );
inline void cMsgINFO( std::string msg ) { cMsgINFO( msg.c_str() ); }

// Print Info in Debug
inline void cMsgDBG( LPCSTR format, ... );
inline void cMsgDBG( std::string msg ) { cMsgDBG( msg.c_str() ); }

// Print Info in Debug and Release
inline void cMsgCRT( LPCSTR format, ... );
inline void cMsgCRT( std::string msg ) { cMsgCRT( msg.c_str() ); }


//****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\*/
/*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*///


// IO funcs


// Check if a file exists
inline bool sFileExists( LPCSTR FileName );

// Check if a file exists
inline bool FileExists( std::string FileName );

// Get a list of file located in PATH with EXT i.e ".ogg"
inline std::vector< std::string > ListFiles( std::string strPath, LPCSTR ExtFile );


//****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\*/
/*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*///

// String manipulation

// Remove extension from FILENAME
inline std::string RemoveExtension( std::string FileName );

// Get EXTENSION from FilePath
inline std::string GetExtFromFilePath( std::string FilePath );

// Get NAME from FilePath
inline std::string GetNameFromFilePath( std::string FilePath, bool bExt = false );

// Get PATH from filePath
inline std::string GetPathFromFilePath( std::string FilePath, bool Slash = true );

// Replace all '\\' and '/' with a dot
inline std::string String_Transform( std::string FilePath );

// Replace all dots with '\\'
inline std::string String_TransformToStd( std::string FilePath );

// Trim a string from left side
// Clean every space after first alpha
inline void String_TrimL( std::string &String );

// Trim a string from right side
// Clean every space before last alpha
inline void String_TrimR( std::string &String );

// Trim a string by left and by right
inline std::string &String_Trim( std::string &String );

// If semicolon is found, cuts string till there, semicolon apart
inline void String_CleanComments( std::string &String );

// Return true if string contains al last one alpha char
inline bool String_ContainsAlpha( const std::string &String );

// Return true if string contains al last one digit char ( 0123456789 )
inline bool String_ContainsDigit( const std::string &String );

// clean comments and check if str contains chars or digits
inline bool String_IsValid( std::string &String );

// If first char is not capital, then make it capital
inline void String_CapitalizeFirstChar( std::string &String );

#define TYPE_KEYVALUE_NONE 0
#define TYPE_KEYVALUE_LISTKEY 1
#define TYPE_KEYVALUE_KEYVALUE 2

typedef struct {
	std::string Key;
	std::string Value;
	char iType;
	bool IsOK;
} KeyValue_t;

// Read string coreectly if format is Key = Value ; comment( optional )
// Returns a struct with key and value as members if read success
inline KeyValue_t GetKeyValue( std::string Line );


//****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\*/
/*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*///

// Conversions

// Create a 2D vector from string
inline Leadwerks::Vec2 StrToVec2( std::string string );

// Create a 2D vector from string
inline Leadwerks::Vec3 StrToVec3( std::string string );

// Return an std vector with 1 = x, 2 = y, 3 = z;
inline std::vector < float > LeadwerksV3ToSTDVec( Leadwerks::Vec3 v );


//****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\*/
/*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*///

// Math

#define EPS_S 0.0000001f
#define EPS 0.0000100f
#define EPS_L 0.0010000f

#define M_PI 3.1415926535897932384626433832795f
#define PI 3.1415926535897932384626433832795f
#define PI_MUL_2 6.2831853071795864769252867665590f
#define PI_MUL_3 9.4247779607693797153879301498385f
#define PI_MUL_4 12.566370614359172953850573533118f
#define PI_DIV_3 1.0471975511965977461542144610932f
#define PI_MUL_6 18.849555921538759430775860299677f
#define PI_MUL_8 25.132741228718345907701147066236f
#define PI_MUL_2 6.2831853071795864769252867665590f
#define PI_DIV_2 1.5707963267948966192313216916398f
#define PI_DIV_4 0.7853981633974483096156608458199f
#define PI_DIV_6 0.5235987755982988730771072305466f
#define PI_DIV_8 0.3926990816987241548078304229099f

// ( Value, Min, Max )
#define L_CLAMP Leadwerks::Math::Clamp

// ( Target, Current, Divisions )
#define L_CURVE Leadwerks::Math::Curve

// ( Start, Stop, Interpolant )
#define L_LERP  Leadwerks::Math::Lerp

// Return 1 for numbers more than zero otherwise -1
inline char Sign( float n );

// Return if abs of a is really near to b
inline bool Similar( float a, float b, float c = EPS );

// Return if abs of a is really near to Zero
inline bool SimilarZero( float a, float cmp = EPS_S );

// Return an average of values contained in vector
// only accepts float vectors!!
inline float fAverage( std::vector< float > Values );

// Return an average of vector contained in vector
inline Leadwerks::Vec3 Vec3Average( std::vector < Leadwerks::Vec3 > Values );

//return clamped vector between min and max for x, y, and z
inline Leadwerks::Vec3 Vec3Clamp ( Leadwerks::Vec3 v, float min, float max );

// return clamped vector between corrisponding min e max for x, y and z
inline Leadwerks::Vec3 Vec3Clamp ( Leadwerks::Vec3 v, float minx, float maxx, float miny, float maxy, float minz, float maxz );

// Curve an entire vector to fVal with f divisions
inline Leadwerks::Vec3 Vec3Curve( float f, Leadwerks::Vec3 v, float d );

// Curve an entire vector to fVal with x, y and z custom divisions
inline Leadwerks::Vec3 Vec3Curve( float f, Leadwerks::Vec3 v, float dX, float dY, float dZ );

// Curve an entire vector from v2 to v1 with f divisions
inline Leadwerks::Vec3 Vec3Curve( Leadwerks::Vec3 v1, Leadwerks::Vec3 v2, float d );

// Curve an entire vector from v2 to v1 with x, y and z custom divisions
inline Leadwerks::Vec3 Vec3Curve( Leadwerks::Vec3 v1, Leadwerks::Vec3 v2, float dX, float dY, float dZ );

//
// https://keithmaggio.wordpress.com/2011/02/15/math-magician-lerp-slerp-and-nlerp/
//
// Interpolates between the vectors a and b by the interpolant t
inline Leadwerks::Vec3 Vec3Lerp( Leadwerks::Vec3 a, Leadwerks::Vec3 b, float t );

// Interpolates between the vectors a and b by the interpolant tX, tY and tZ
inline Leadwerks::Vec3 Vec3LerpXYZ( Leadwerks::Vec3 a, Leadwerks::Vec3 b, float tX, float tY, float tZ );

// Interpolates between the vectors a and b by the interpolant t
// Slerp is useful for rotation, mostly
inline Leadwerks::Vec3 Vec3SLerp( Leadwerks::Vec3 a, Leadwerks::Vec3 b, float t );

// Interpolates between max three vectors by the interpolant t
inline Leadwerks::Vec3 Vec3MultiLerp( std::vector < Leadwerks::Vec3 > &vVectors, float t = 0.5 );

// Interpolates between max three vectors by the interpolant t
// Is useful for rotation, but is very 
inline Leadwerks::Vec3 Vec3MultiSLerp( std::vector < Leadwerks::Vec3 > &vVectors, float t = 0.5 );


//****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\/****\*/
/*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*//*...*///

// Assignaments

// Assign x and y to x and y menbers of Vec2
inline void AssignVec2To( Leadwerks::Vec2 &Vec2, float x, float y );

// Assign x and y to x and y menbers of Vec2
inline void AssignVec2To( Leadwerks::Vec2 *Vec2, float x, float y );

// Assign x, y and z to x, y and z menbers of Vec3
inline void AssignVec3To( Leadwerks::Vec3 &Vec3, float x, float y, float z );

// Assign x, y and z to x, y and z menbers of Vec3
inline void AssignVec3To( Leadwerks::Vec3 *Vec3, float x, float y, float z );

// Usefull for multi assignaments


// Assign v1 and v2 to V1 and V2
inline void SetInt2( int &V1, int &V2, int v1, int v2 );

// Assign v1, v2 and v3 to V1, V2 and V3
inline void SetInt3( int &V1, int &V2, int &V3, int v1, int v2, int v3 );

// Assign v1 and v2 to V1 and V2
inline void SetBool2( bool &V1, bool &V2, bool v1, bool v2 );

// Assign v1, v2 and v3 to V1, V2 and V3
inline void SetBool3( bool &V1, bool &V2, bool &V3, bool v1, bool v2, bool v3 );

// Assign v1 and v2 to V1 and V2
inline void SetFloat2( float &V1, float &V2, float v1, float v2 );

// Assign v1, v2 and v3 to V1, V2 and V3
inline void SetFloat3( float &V1, float &V2, float &V3, float v1, float v2, float v3 );

// Assign v1 and v2 to V1 and V2
inline void SetString2( std::string &V1, std::string &V2, std::string v1, std::string v2 );

// Assign v1, v2 and v3 to V1, V2 and V3
inline void SetString3( std::string &V1, std::string &V2, std::string &V3, std::string v1, std::string v2, std::string v3 );

#include "Utils_inline.h"