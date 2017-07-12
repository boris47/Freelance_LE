
#pragma once

cLineValue *cSection::GetLineValue( std::string Key ) {
	
	if ( vSection.empty() ) return NULL;

	cLineValue *pLineValue = NULL;
	for ( auto LineValue : vSection )  {
		if ( LineValue->IsKey( Key ) ) { pLineValue = LineValue; }
	}

	return pLineValue;
}


bool cSection::HasKey( std::string Key ) {
	
	if ( vSection.empty() ) return false;

	for ( auto pLineValue : vSection )  {
		if ( pLineValue->IsKey( Key ) ) return true;
	}

	return false;
}


int cSection::KeyType( std::string Key ) {

	if ( vSection.empty() ) return LTX_READER::VALUE_TYPE::NONE;

	cLineValue *pLineValue = NULL; cValue* Value = NULL;
	if ( pLineValue = GetLineValue( Key ) ) {
		int iType = pLineValue->Type();
		switch( iType ) {

			case LTX_READER::VALUE_TYPE::NONE: {
				cMsgCRT( "cSection::KeyType:WARNING a key has type NONE !!!" );
				return iType;
			}

			case LTX_READER::VALUE_TYPE::SINGLE: {
				if ( Value = pLineValue->GetValue() ) { return Value->Type(); }
				cMsgCRT( "cSection::KeyType:WARNING a key has no value but designed as SINGLE !!!" );
				break;
			}

			case LTX_READER::VALUE_TYPE::MULTI: {
				if ( pLineValue->GetMultiValue() ) return iType;
				cMsgCRT( "cSection::KeyType:WARNING a key has no value but designed as MULTI !!!" );
				break;
			}

			case LTX_READER::VALUE_TYPE::KEYONLY: {
				cMsgCRT( "cSection::KeyType:WARNING a key has value type as KEYONLY !!!" );
				return LTX_READER::VALUE_TYPES::NONE;
			}
		}
	}

	return LTX_READER::VALUE_TYPES::NONE;
}

void cSection::Add( cLineValue *LineValue ) {
	if ( LineValue && ( ( LineValue->Type() == LTX_READER::VALUE_TYPE::SINGLE ) || ( LineValue->Type() == LTX_READER::VALUE_TYPE::MULTI ) ) )
		vSection.push_back( LineValue );
}


std::string cSection::GetRawValue(std::string Key, std::string	Default  ) {

	cLineValue *pLineValue = NULL;
	return ( pLineValue = GetLineValue( Key ) ) ? pLineValue->RawValue() : Default;

}


int cSection::GetInt( std::string Key, int Default ) {

	cLineValue *pLineValue = NULL;
	return ( pLineValue = GetLineValue( Key ) ) ? *pLineValue->GetValue() : Default;

}
		

bool cSection::bGetInt( std::string Key, int &Out, int Default ) {

	cLineValue *pLineValue = NULL;
	if ( pLineValue = GetLineValue( Key ) ) {
		if ( pLineValue->Type() == LTX_READER::VALUE_TYPE::SINGLE ) {
			Out = *pLineValue->GetValue();
			return true;
		}
	}

	Out = Default;
	return false;
}


bool cSection::GetBool( std::string Key, bool Default ) {

	cLineValue *pLineValue = NULL;
	return ( pLineValue = GetLineValue( Key ) ) ? *pLineValue->GetValue() : Default;

}

bool cSection::bGetBool( std::string Key, bool &Out, bool Default ) {

	cLineValue *pLineValue = NULL;
	if ( pLineValue = GetLineValue( Key ) ) {
		if ( pLineValue->Type() == LTX_READER::VALUE_TYPE::SINGLE ) {
			Out = *pLineValue->GetValue();
			return true;
		}
	}

	Out = Default;
	return false;
}


float cSection::GetFloat( std::string Key, float Default ) {

	cLineValue *pLineValue = NULL;
	return ( pLineValue = GetLineValue( Key ) ) ? *pLineValue->GetValue() : Default;

}

bool cSection::bGetFloat( std::string Key, float &Out, float Default ) {

	cLineValue *pLineValue = NULL;
	if ( pLineValue = GetLineValue( Key ) ) {
		if ( pLineValue->Type() == LTX_READER::VALUE_TYPE::SINGLE ) {
			Out = *pLineValue->GetValue();
			return true;
		}
	}

	Out = Default;
	return false;
}


std::string cSection::GetString( std::string Key, std::string Default ) {

	cLineValue *pLineValue = NULL;
	 return ( pLineValue = GetLineValue( Key ) ) ? *pLineValue->GetValue() : Default;

}

bool cSection::bGetString( std::string Key, std::string &Out, std::string Default ) {

	cLineValue *pLineValue = NULL;
	if ( pLineValue = GetLineValue( Key ) ) {
		if ( pLineValue->Type() == LTX_READER::VALUE_TYPE::SINGLE ) {
			Out = *pLineValue->GetValue();
			return true;
		}
	}

	Out = Default;
	return false;
}

size_t cSection::GetMultiSize( std::string Key ) {

	cLineValue *pLineValue = NULL; cMultiValue* pMultiValue = NULL;
	return ( ( pLineValue = GetLineValue( Key ) ) && ( pMultiValue = pLineValue->GetMultiValue() ) ) ? ( pMultiValue->Size() + 1 ) : 0;

}

cValue& cSection::GetMultiValue( std::string Key, size_t Index, int Type ) {

	pDefaultCValue->SetType( Type );
	cLineValue *pLineValue = NULL; cMultiValue* pMultiValue = NULL;
	return ( ( pLineValue = GetLineValue( Key ) ) && ( pMultiValue = pLineValue->GetMultiValue() ) ) ?
		*pMultiValue->At( Index - 1 ) : *pDefaultCValue;

}

bool cSection::bGetMultiValue( std::string Key, cValue *&Out, size_t Index, int Type ) {

	pDefaultCValue->SetType( Type );
	cLineValue *pLineValue = NULL; cMultiValue* pMultiValue = NULL; cValue *pValue = NULL;
	if ( pLineValue = GetLineValue( Key ) ) {
		if ( pMultiValue = pLineValue->GetMultiValue() ) {
			if ( pValue = pMultiValue->At( Index - 1 ) ) {
				Out = pValue;
				return true;
			}
		}
	}

	Out = pDefaultCValue;
	return false;
}



Leadwerks::Vec2 cSection::GetVec2( std::string Key ) {

	cLineValue *pLineValue = NULL; cMultiValue* pMultiValue = NULL;
	if ( ( pLineValue = GetLineValue( Key ) ) && ( pMultiValue = pLineValue->GetMultiValue() ) ) {
		return Leadwerks::Vec2( *pMultiValue->At( 0 ), *pMultiValue->At( 1 ) );
	}
	return Leadwerks::Vec2( 0.0, 0.0 );

}

bool cSection::bGetVec2( std::string Key, Leadwerks::Vec2 &Out, Leadwerks::Vec2 Default ) {

	cLineValue *pLineValue = NULL; cMultiValue* pMultiValue = NULL;
	cValue *pValue1 = NULL; cValue *pValue2 = NULL;
	if ( pLineValue = GetLineValue( Key ) ) {
		if ( pMultiValue = pLineValue->GetMultiValue() ) {
			if ( ( pValue1 = pMultiValue->At( 0 ) ) && ( pValue2 = pMultiValue->At( 1 ) ) ) {
				Out = Leadwerks::Vec2( *pValue1, *pValue2 );
				return true;
			}
		}
	}

	Out = Default;
	return false;
}

Leadwerks::Vec3 cSection::GetVec3( std::string Key ) {

	cLineValue *pLineValue = NULL; cMultiValue* pMultiValue = NULL;
	if ( ( pLineValue = GetLineValue( Key ) ) && ( pMultiValue = pLineValue->GetMultiValue() ) ) {
		return Leadwerks::Vec3( *pMultiValue->At( 0 ), *pMultiValue->At( 1 ), *pMultiValue->At( 2 ) );
	};
	return Leadwerks::Vec3( 0.0, 0.0, 0.0 );

}

bool cSection::bGetVec3( std::string Key, Leadwerks::Vec3 &Out, Leadwerks::Vec3 Default ) {

	cLineValue *pLineValue = NULL; cMultiValue* pMultiValue = NULL;
	cValue *pValue1 = NULL; cValue *pValue2 = NULL; cValue *pValue3 = NULL;
	if ( pLineValue = GetLineValue( Key ) ) {
		if ( pMultiValue = pLineValue->GetMultiValue() ) {
			if ( ( pValue1 = pMultiValue->At( 0 ) ) && ( pValue2 = pMultiValue->At( 1 ) ) && ( pValue3 = pMultiValue->At( 2 ) ) ) {
				Out = Leadwerks::Vec3( *pValue1, *pValue2, *pValue3 );
				return true;
			}
		}
	}

	Out = Default;
	return false;
}

Leadwerks::Vec4 cSection::GetVec4( std::string Key ) {

	cLineValue *pLineValue = NULL; cMultiValue* pMultiValue = NULL;
	if ( ( pLineValue = GetLineValue( Key ) ) && ( pMultiValue = pLineValue->GetMultiValue() ) ) {
		return Leadwerks::Vec4( *pMultiValue->At( 0 ), *pMultiValue->At( 1 ), *pMultiValue->At( 2 ), *pMultiValue->At( 3 ) );
	};
	return Leadwerks::Vec4( 0.0, 0.0, 0.0, 0.0 );

}

bool cSection::bGetVec4( std::string Key, Leadwerks::Vec4 &Out, Leadwerks::Vec4 Default ) {

	cLineValue *pLineValue = NULL; cMultiValue* pMultiValue = NULL;
	cValue *pValue1 = NULL; cValue *pValue2 = NULL; cValue *pValue3 = NULL; cValue *pValue4 = NULL;
	if ( pLineValue = GetLineValue( Key ) ) {
		if ( pMultiValue = pLineValue->GetMultiValue() ) {
			if ( ( pValue1 = pMultiValue->At( 0 ) ) &&
				( pValue2 = pMultiValue->At( 1 ) ) &&
				( pValue3 = pMultiValue->At( 2 ) ) &&
				( pValue4 = pMultiValue->At( 3 ) ) ) {
				Out = Leadwerks::Vec4( *pValue1, *pValue2, *pValue3, *pValue4 );
				return true;
			}
		}
	}

	Out = Default;
	return false;
}


void cSection::SetValue( std::string Key, cValue *Value ) {

	cLineValue *pLineValue = GetLineValue( Key );
	// if not exists create one
	if ( !pLineValue ) pLineValue = new cLineValue( Key, LTX_READER::VALUE_TYPE::SINGLE );
	pLineValue->Clear();
	pLineValue->Set( Value );

}

void cSection::SetMultiValue( std::string Key, std::vector < cValue * > vValues ) {

	cLineValue *pLineValue = GetLineValue( Key );
	// if not exists create one
	if ( !pLineValue ) pLineValue = new cLineValue( Key, LTX_READER::VALUE_TYPE::MULTI );
	pLineValue->Clear();
	pLineValue->Set( new cMultiValue( vValues ) );

}

void cSection::SetInt( std::string Key, int Value ) {

	SetValue( Key, new cValue( Value ) );

}

void cSection::SetBool( std::string Key, bool Value ) {

	SetValue( Key, new cValue( Value ) );

}

void cSection::SetFloat( std::string Key, float Value ) {

	SetValue( Key, new cValue( Value ) );

}

void cSection::SetString ( std::string Key, std::string Value ) {

	SetValue( Key, new cValue( Value ) );

}

void cSection::SetVec2( std::string Key, Leadwerks::Vec2 &Vec ) {

	cLineValue *pLineValue = GetLineValue( Key );
	// if not exists create one
	if ( !pLineValue ) pLineValue = new cLineValue( Key, LTX_READER::VALUE_TYPE::MULTI );
	pLineValue->Clear();

	std::vector < cValue *> vValues( { new cValue( Vec.x ), new cValue( Vec.y ) } );
	pLineValue->Set( new cMultiValue( vValues ) );

}

void cSection::SetVec3( std::string Key, Leadwerks::Vec3 &Vec ) {

	cLineValue *pLineValue = GetLineValue( Key );
	// if not exists create one
	if ( !pLineValue ) pLineValue = new cLineValue( Key, LTX_READER::VALUE_TYPE::MULTI );
	pLineValue->Clear();

	std::vector < cValue *> vValues( { new cValue( Vec.x ), new cValue( Vec.y ), new cValue( Vec.z ) } );
	pLineValue->Set( new cMultiValue( vValues ) );

}

void cSection::Print( void ) {

	for ( auto pLineValue : vSection )  {
		cMsgDBG( "    [%s] = { ", pLineValue->Key().c_str() );
		pLineValue->Print();
		cMsgDBG( "    }");
	}

}

