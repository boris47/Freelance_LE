
#include "LiveEntity.h"


void cLiveEntity::SetMotionType( int i ) {

	iPrevMotionType = iMotionType;
	switch( i ) {

		case LIVE_ENTITY::Motion::Walk:		{ this->SetPlainWalking(); return; }
		case LIVE_ENTITY::Motion::Fly:		{ this->SetFlying(); return; }
		case LIVE_ENTITY::Motion::Swim:		{ this->SetSwimming(); return; }
		case LIVE_ENTITY::Motion::P1ToP2:	{ this->SetClimbing(); return; }
	}

}


void cLiveEntity::SetPlainWalking() {

	if ( iMotionType == LIVE_ENTITY::Motion::Walk ) return;

	iPrevMotionType = iMotionType;
	iMotionType = LIVE_ENTITY::Motion::Walk;

	this->ClearState();
	pEntity->SetGravityMode( true );
	pEntity->SetFriction( 0.0f, 1.0f );

	Engine()->InputManager()->SetHoldCrouch( false );
	Engine()->InputManager()->SetHoldJump( false );
	Engine()->InputManager()->SetHodRun( true );

}

void cLiveEntity::SetFlying() {

	if ( iMotionType == LIVE_ENTITY::Motion::Fly ) return;
	
	iPrevMotionType = iMotionType;
	iMotionType = LIVE_ENTITY::Motion::Fly;

	this->ClearState();
	pEntity->SetGravityMode( false );
	pEntity->SetFriction( 0.0f, 10.0f );

	Engine()->InputManager()->SetHoldCrouch( true );
	Engine()->InputManager()->SetHoldJump( true );
	Engine()->InputManager()->SetHodRun( true );

}

void cLiveEntity::SetSwimming() {

	if ( iMotionType == LIVE_ENTITY::Motion::Swim ) return;

	iPrevMotionType = iMotionType;
	iMotionType = LIVE_ENTITY::Motion::Swim;

	this->ClearState();
	SetCrouched( true );

	pEntity->SetGravityMode( false );
	pEntity->SetFriction( 0.1f, 0.1f ) ;
//	pEntity->SetVelocity( EMPTY_VEC3 );
	
	Engine()->InputManager()->SetHoldCrouch( true );
	Engine()->InputManager()->SetHoldJump( true );
	Engine()->InputManager()->SetHodRun( true );
	
}

void cLiveEntity::SetClimbing() {

	if ( iMotionType == LIVE_ENTITY::Motion::P1ToP2 ) return;

	Stop();
///	Stop();
///	Stop();
///	Stop();
	
	iPrevMotionType = iMotionType;
	iMotionType = LIVE_ENTITY::Motion::P1ToP2;

	bool b = IsCrouched();
	this->ClearState();
	SetCrouched( b );
	
	pEntity->SetGravityMode( false );
	pEntity->SetFriction( 0.0f, 1000.0f );

	Engine()->InputManager()->SetHoldCrouch( false );
	Engine()->InputManager()->SetHoldJump( false );
	Engine()->InputManager()->SetHodRun( true );

}