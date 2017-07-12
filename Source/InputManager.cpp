

#include "InputManager.h"
#include "Utils\Utils.h"

#define INPUT_FORWARD			Leadwerks::Key::W
#define INPUT_FORWARD_ALT		Leadwerks::Key::Up

#define INPUT_BACKWARD			Leadwerks::Key::S
#define INPUT_BACKWARD_ALT		Leadwerks::Key::Down

#define INPUT_STRAFE_LEFT		Leadwerks::Key::A
#define INPUT_STRAFE_LEFT_ALT	Leadwerks::Key::Left

#define INPUT_STRAFE_RIGHT		Leadwerks::Key::D
#define INPUT_STRAFE_RIGHT_ALT	Leadwerks::Key::Right

#define INPUT_LEAN_LEFT			Leadwerks::Key::Q
#define INPUT_LEAN_LEFT_ALT		Leadwerks::Key::NumPad7

#define INPUT_LEAN_RIGHT		Leadwerks::Key::E
#define INPUT_LEAN_RIGHT_ALT	Leadwerks::Key::NumPad9

#define INPUT_CROUCH_HOLD		false
#define INPUT_CROUCH			Leadwerks::Key::ControlKey
#define INPUT_CROUCH_ALT		Leadwerks::Key::RControlKey

#define INPUT_JUMP_HOLD			false
#define INPUT_JUMP				Leadwerks::Key::Space
#define INPUT_JUMP_ALT			Leadwerks::Key::NumPad0

#define INPUT_RUN_HOLD			true
#define INPUT_RUN				Leadwerks::Key::Shift

#define INPUT_USE				Leadwerks::Key::F
#define INPUT_USE_ALT			Leadwerks::Key::Enter

#define INPUT_ITEM1				Leadwerks::Key::F1
#define INPUT_ITEM2				Leadwerks::Key::F2
#define INPUT_ITEM3				Leadwerks::Key::F3
#define INPUT_ITEM4				Leadwerks::Key::F4

cInputManager::cInputManager() {

	if ( !( pWindow = Leadwerks::Window::GetCurrent() ) ) {
		cMsgCRT( "cInputManager::Constructor:Cannot initialize cinputManager without a window" );
		return; // leave bIsOK == false
	}
	bIsOK = true;

	bCrouchPuttonHold	= INPUT_CROUCH_HOLD;
	bJumpButtonHold		= INPUT_JUMP_HOLD;
	bRunButtonHold		= INPUT_RUN_HOLD;

}

void cInputManager::Update( void ) {

	Inputs.Reset();
	
	Inputs.Forward = ( pWindow->KeyDown( INPUT_FORWARD ) || pWindow->KeyDown( INPUT_FORWARD_ALT ) );
	Inputs.Backward = ( pWindow->KeyDown( INPUT_BACKWARD ) || pWindow->KeyDown( INPUT_BACKWARD_ALT ) );
	Inputs.StrafeLeft = ( pWindow->KeyDown( INPUT_STRAFE_LEFT ) || pWindow->KeyDown( INPUT_STRAFE_LEFT_ALT ) );
	Inputs.StrafeRight = ( pWindow->KeyDown( INPUT_STRAFE_RIGHT ) || pWindow->KeyDown( INPUT_STRAFE_RIGHT_ALT ) );

	Inputs.LeanLeft = ( pWindow->KeyDown( INPUT_LEAN_LEFT ) || pWindow->KeyDown( INPUT_LEAN_LEFT_ALT ) );
	Inputs.LeanRight = ( pWindow->KeyDown( INPUT_LEAN_RIGHT ) || pWindow->KeyDown( INPUT_LEAN_RIGHT_ALT ) );

	Inputs.Crouch = bCrouchPuttonHold ?
		( pWindow->KeyDown( INPUT_CROUCH ) || pWindow->KeyDown( INPUT_CROUCH_ALT ) )
	:
		( pWindow->KeyHit( INPUT_CROUCH ) || pWindow->KeyHit( INPUT_CROUCH_ALT ) )
	;
	
	
	Inputs.Jump = bJumpButtonHold ?
		( pWindow->KeyDown( INPUT_JUMP ) || pWindow->KeyDown( INPUT_JUMP_ALT ) )
	:
		( pWindow->KeyHit( INPUT_JUMP ) || pWindow->KeyHit( INPUT_JUMP_ALT ) )
	;
	
	Inputs.Run = bRunButtonHold ? pWindow->KeyDown( INPUT_RUN ) : pWindow->KeyHit( INPUT_RUN );

	Inputs.Use = ( pWindow->KeyHit( INPUT_USE ) || pWindow->KeyHit( INPUT_USE_ALT ) );

	Inputs.Item1 = pWindow->KeyHit( INPUT_ITEM1 );
	Inputs.Item2 = pWindow->KeyHit( INPUT_ITEM2 );
	Inputs.Item3 = pWindow->KeyHit( INPUT_ITEM3 );
	Inputs.Item4 = pWindow->KeyHit( INPUT_ITEM4 );

}






















