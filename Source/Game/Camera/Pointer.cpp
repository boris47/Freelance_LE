

#include "Pointer.h"
#include "Game\Level\World.h"


// Constructor
cPointer::cPointer( Leadwerks::Entity *SrcEntity ) {

	if ( !SrcEntity ) return;

	pEntity = SrcEntity;

}


// Public
bool cPointer::SetSource( Leadwerks::Entity *Entity ) {

	if ( !Entity ) return false;

	pEntity = Entity;
	return true;

}


// Public
bool cPointer::SetPosition( Leadwerks::Vec3 Position ) {

	if ( !Position.Length() ) return false;

	vPosition = Position;
	return true;

}


// Public
bool cPointer::SetDirection( Leadwerks::Vec3 Direction ) {

	if ( !Direction.Length() ) return false;

	vDirection = Direction.Normalize();
	return true;

}


// Public
bool cPointer::SetMaxDistance( float MaxDistance ) {

	if ( MaxDistance < 0.0f ) return false;

	fMaxDistance = MaxDistance;
	return true;

}


// Public
void cPointer::Update() {

	if ( !pEntity || !vDirection.Length() || ( fMaxDistance <= 0.0f ) ) return;

	this->Reset();

	// Final position
	Leadwerks::Vec3 vRelPos = Leadwerks::Transform::Vector( vDirection * fMaxDistance, pEntity, NULL );
	bHasInfo = false;

	if ( World()->GetWorld()->Pick( vPosition, vPosition + vRelPos, Info ) ) {
		fReachedDistance = vPosition.DistanceToPoint( Info.position );
		bHasInfo = true;
	}

}


// Public
void cPointer::Reset() {

	bHasInfo = false;
	fReachedDistance = fMaxDistance;


}