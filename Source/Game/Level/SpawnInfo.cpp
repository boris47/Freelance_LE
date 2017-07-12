
#include "SpawnInfo.h"

cSpawnInfo::cSpawnInfo() {}

cSpawnInfo::cSpawnInfo( std::string Section, Leadwerks::Vec3 Position, Leadwerks::Vec3 Rotation, Leadwerks::Vec3 Scale ) {
	sSection = Section;
	vPosition = Position;
	vRotation = Rotation;
	vScale = Scale;
}

bool cSpawnInfo::HasNoInfo() {
	return ( lInfos.size() > 0 );
}

void cSpawnInfo::SetInfo( std::string Info ) {
	
	if ( Info.empty() ) return;
	
	size_t Colon = 0, Start = 0, End = std::string::npos;
	for ( size_t i = 0; i < Info.size(); i++ ) {
		if ( Info[ i ]  == ',' ) {
			std::string sInfo = Info.substr( Start, i );
			AddInfo( sInfo );
			Start = i + 1;
			continue;
		}
		std::string sInfo = Info.substr( Start, End );
		AddInfo( sInfo );
	}
}

void cSpawnInfo::AddInfo( std::string Info ) {
	lInfos.push_back( Info );
}

bool cSpawnInfo::FindInfo( std::string Info ) {
	for ( auto sInfo : lInfos ) if ( sInfo == Info ) return true;
	return false;
}

std::string cSpawnInfo::GetInfo( std::string Info ) {
	for ( auto sInfo : lInfos ) if ( sInfo == Info ) return sInfo;
	return std::string( "" );
}

std::string cSpawnInfo::GetInfoValue( std::string Info ) {

	std::string sInfo = "";
	for ( auto s : lInfos ) if ( s == Info ) sInfo = s;

	if ( sInfo.empty() ) return "";
	
	size_t EqualSign = 0;
	for ( size_t i = 0; i < sInfo.size(); i++ ) {
		if ( Info[ i ]  == '=' ) { EqualSign = i; continue; }
	}
	
	return ( EqualSign ) ? String_Trim( sInfo.substr( EqualSign + 1 , std::string::npos ) ) : sInfo; // useful for a list
}

void cSpawnInfo::SubInfo( std::string Info ) {
	for ( std::list < std::string >::iterator it = lInfos.begin(); it != lInfos.end(); ++it ) {
		if ( *it == Info ) { lInfos.erase( it ); break; }
	}
}