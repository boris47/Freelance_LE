#ifndef MULTIVALUE_H
#define MULTIVALUE_H
#pragma once

#include <vector>

#include "Value.hpp"

class cMultiValue {
	private:
		std::vector< cValue * > vValues;
		
	public:
		typedef std::vector< cValue * >::const_iterator vValuesCI;
		
		cMultiValue( void ) { vValues.clear(); };
		cMultiValue( std::vector< cValue * > Values ) { vValues.clear(); vValues = Values; };
		~cMultiValue( void ) {
			for( auto it = Begin(); it != End(); it++ ) delete ( *it );
			vValues.clear();
		};
		
		inline void			Add( cValue *Value );
		inline void			Set( std::vector< cValue * > Vector );
		
		inline size_t		Size( void );
		inline vValuesCI	Begin( void ) const;
		inline vValuesCI	End( void ) const;
		inline cValue *		At( size_t Index );
		
		inline void			Print( void );
	
};


void					cMultiValue::Add( cValue *Value ) { vValues.push_back( Value ); };
void					cMultiValue::Set( std::vector< cValue * > Vector ) { vValues = Vector; };

size_t					cMultiValue::Size( void ) { return vValues.size(); };
cMultiValue::vValuesCI	cMultiValue::Begin( void ) const { return vValues.begin(); };
cMultiValue::vValuesCI	cMultiValue::End( void ) const { return vValues.end(); };
cValue *				cMultiValue::At( size_t Index ) { try { return vValues.at( Index ); } catch ( ... ) { return NULL; }; }

void					cMultiValue::Print( void ) { for ( const auto &Value : vValues ) Value->Print(); };


#endif//MULTIVALUE_H