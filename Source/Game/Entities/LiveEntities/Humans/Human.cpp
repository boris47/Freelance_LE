
#include "Human.h"
#include "Game\Camera\CamManager.h"


bool cHuman::SetDragging( Leadwerks::Entity *e ) {

	{
		Dragged.pEntity = e;
		Dragged.vPosition = Leadwerks::Transform::Point(    e->GetPosition( true ),   NULL, CamManager()->GetCamera() );
		Dragged.vRotation = Leadwerks::Transform::Rotation( e->GetQuaternion( true ), NULL, CamManager()->GetCamera() );
		Dragged.iCollisionOrig = e->GetCollisionType();
		e->SetCollisionType( Leadwerks::Collision::Debris );
	}
	return true;

}

void cHuman::DropEntityDragged( void ) {

	if ( !Dragged.pEntity ) return;

	Dragged.pEntity->SetCollisionType( Dragged.iCollisionOrig );

	if ( Dragged.pEntity->ContainsFunction( "OnDrop" ) )
		Dragged.pEntity->CallFunction( "OnDrop" );

	Dragged.pEntity = NULL;

}