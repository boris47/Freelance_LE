
#pragma once

#include <Engine.h>

namespace ENTITY {
	enum Type {
		NONE,
		ACTOR,
		HUMAN,
		ANIMAL,
		OBJECT
	};
};

// forwrad decalarations
class cLiveEntity;
class cHuman;

class cEntity : public cStateClass, public cEntitySection, public cCallbackOnce {
	protected:
		// 0 to 4,294,967,295
		unsigned long int iID				= 0;

		long iLastLandTime					= 0;
		
		std::string sName					= "";
		std::string sSection				= "";

		float fPrevMass						= ZERO_FLOAT;

		Leadwerks::Entity *pParent			= NULL;
		Leadwerks::Entity *pEntity			= NULL;
		
		int iEntityType						= ENTITY::Type::NONE;

		bool	bIsInWater					= false;
		bool	bIsUnderWater				= false;
		
		Leadwerks::Vec3 vRotationP1ToP2		= EMPTY_VEC3;

		std::list < std::string >			lInfos;

		bool bIsOK							= false;
	
	public:
		
		virtual	~cEntity() {};

		inline	bool					IsOK()					{ return bIsOK; }

		inline	unsigned long int		ID()					{ return iID; }

		inline	std::string				Name() const 			{ return sName; }
		inline	std::string				Section()				{ return GetSection()->Name(); }

		inline	void					SetMass( float v )		{ fPrevMass = pEntity->GetMass(); pEntity->SetMass( v ); }
		inline	float					GetMass( void )			{ return pEntity->GetMass(); }
		inline	float					GetPrevMass( void )		{ return fPrevMass; }

		inline	void					SetParent( Leadwerks::Entity *e ) { pParent = e; }
		inline	Leadwerks::Entity *		GetParent()				{ return pParent; }

		inline	Leadwerks::Entity *		GetEntity()				{ return pEntity; }

				bool					IsLiveEntity();
				cLiveEntity *			GetAsLiveEntity();

				bool					IsHuman();
				cHuman *				GetAsHuman();

				void					Stop();

		inline	void					SetType( int Type  )	{ iEntityType = Type; }
		inline	int						GetType()				{ return iEntityType; }

		inline	void					SetInWater( bool b )	{ bIsInWater = b; }
		inline	bool					IsInWater( void )		{ return bIsInWater; }

		inline	void					SetUnderWater( bool b )	{ bIsUnderWater = b; }
		inline	bool					IsUnderWater( void )	{ return bIsUnderWater; }

		inline	void					SetRotationP1ToP2( Leadwerks::Vec3 v ) { vRotationP1ToP2 = v; }
		inline	Leadwerks::Vec3			GetRotationP1ToP2()		{ return vRotationP1ToP2; }
		
		inline	bool					IsAirborne()			{ return ( abs( pEntity->GetVelocity().y ) > 0.0f ); }
				float					GetAirbourneHeigth();

		virtual	void					Update( void )			= 0;
		virtual void					OnFrame( void )			= 0;

};

