
#pragma once

#include "Leadwerks.h"

class cInputManager {
	private:

		bool bCrouchPuttonHold	= false;
		bool bJumpButtonHold	= false;
		bool bRunButtonHold		= true;

		struct inputs_t {
			bool Forward, Backward, StrafeLeft, StrafeRight;
			bool LeanLeft, LeanRight;
			bool Crouch, Jump, Run;
			bool Use;
			bool Item1, Item2, Item3, Item4;

			void Reset() {
				Forward = Backward = StrafeLeft = StrafeRight =
				LeanLeft = LeanRight =
				Crouch = Jump = Run =
				Use =
				Item1 = Item2 = Item3 = Item4 = false;
			}

		} Inputs;

		Leadwerks::Window *pWindow = nullptr;
		bool bIsOK = false;

	public:

		cInputManager();

	public:

		inline	void SetHoldCrouch( bool b ){ bCrouchPuttonHold = b;	}
		inline	void SetHoldJump( bool b )	{ bJumpButtonHold = b;		}
		inline	void SetHodRun( bool b )	{ bRunButtonHold = b;		}

		inline	bool GetHoldCrouch( void )	{ return bCrouchPuttonHold; }
		inline	bool GetHoldJump( void )	{ return bJumpButtonHold; }
		inline	bool GetHodRun( void )		{ return bRunButtonHold; }

		inline	bool Forward()				{ return Inputs.Forward;	}
		inline	bool Backward()				{ return Inputs.Backward;	}
		inline	bool StrafeLeft()			{ return Inputs.StrafeLeft;	}
		inline	bool StrafeRight()			{ return Inputs.StrafeRight;}

		inline	bool LeanLeft()				{ return Inputs.LeanLeft;	}
		inline	bool LeanRight()			{ return Inputs.LeanRight;	}

		inline	bool Crouch()				{ return Inputs.Crouch;		}
		inline	bool Jump()					{ return Inputs.Jump;		}
		inline	bool Run()					{ return Inputs.Run;		}

		inline	bool Use()					{ return Inputs.Use;		}

		inline	bool Item1()				{ return Inputs.Item1;		}
		inline	bool Item2()				{ return Inputs.Item2;		}
		inline	bool Item3()				{ return Inputs.Item3;		}
		inline	bool Item4()				{ return Inputs.Item4;		}

				void Update();

};