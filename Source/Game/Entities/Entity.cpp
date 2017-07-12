
#include "Entity.h"
#include "Game\Entities\LiveEntities\LiveEntity.h"
#include "Game\Entities\LiveEntities\Humans\Human.h"

bool cEntity::IsLiveEntity() {
	return ( GET_AS( this, cLiveEntity ) != NULL );
}

bool cEntity::IsHuman() {
	return ( GET_AS( this, cHuman ) != NULL );
}

cLiveEntity * cEntity::GetAsLiveEntity() {
	return GET_AS( this, cLiveEntity );
}

cHuman * cEntity::GetAsHuman() {
	return GET_AS( this, cHuman );
}

void cEntity::Stop() {

		Leadwerks::NewtonDynamicsBody *pBody = dynamic_cast< Leadwerks::NewtonDynamicsBody *>( pEntity->body );
		if ( pBody ) {
		
		//	pBody->body
//			float pZeroVec[ 3 ] = { 0.0f, 0.0f, 0.0f };
//			NewtonBodySetVelocity( pBody->body, pZeroVec );
//			NewtonBodySetForce( pBody->body, pZeroVec );
//			NewtonBodySetTorque( pBody->body, pZeroVec );
			pBody->velocity = EMPTY_VEC3;
			pBody->torque = EMPTY_VEC3;
		}
		pEntity->SetVelocity( EMPTY_VEC3, false );
		pEntity->SetVelocity( EMPTY_VEC3, true );
		
		if ( pEntity->GetPhysicsMode() == Leadwerks::Entity::CharacterPhysics )
			pEntity->SetInput( 0.0f, 0.0f, 0.0f, 0.0f, true, 0.0f, 1000.0f, true );
		pEntity->UpdatePhysics();
///		pEntity->UpdateMatrix();
///		pEntity->UpdateNavigation();
//		pEntity->UpdateLocalOrientation();
//		pEntity->UpdateGlobalOrientation();

}


float cEntity::GetAirbourneHeigth() {

	if ( !IsAirborne() ) return 0.0f;

	Leadwerks::PickInfo pPickInfo = Leadwerks::PickInfo();
	if ( pEntity->world->Pick( pEntity->GetPosition(), pEntity->GetPosition() - Leadwerks::Vec3( 0.0f, 1000.f, 1.0f ), pPickInfo, 0.0f, true, Leadwerks::Collision::LineOfSight ) ) {
		return ( pEntity->GetPosition().DistanceToPoint( pPickInfo.position ) );
	}

	return 0.0f;
}