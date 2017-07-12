
#pragma once

#include <Engine.h>
#include "Inventory.h"
#include "FootstepManager.h"
#include "Game\Entities\Entity.h"
#include "Game\Entities\Item\Usables\FlashLight.h"

#define CROUCH_JUMP_BOOST 1.5
#define FORWARD_MUL  1.0
#define BACKWARD_MUL 0.8
#define STRAFE_MUL	 0.6
#define SPEEDHACK_MUL 0.70710678

namespace LIVE_ENTITY {
	enum Actions {
		Moving		= 1 << 0,

		Leaning		= 1 << 1,

		Walking		= 1 << 2,
		Running		= 1 << 3,

		Jumping		= 1 << 4,
		Hanging		= 1 << 5,
		Falling		= 1 << 6,

		Crouched	= 1 << 7,
	};

	enum Motion {
		None		= 1 << 0,
		Walk		= 1 << 1,
		Fly			= 1 << 2,
		Swim		= 1 << 3,
		P1ToP2		= 1 << 4
	};
};

struct Entity_States_Temp {

	bool bIsCrouched	= false;

	bool bIsMoving		= false;
	bool bIsWalking		= false;
	bool bIsRunning		= false;

	bool bIsJumping		= false;
	bool bIsHanging		= false;
	bool bIsFalling		= false;

	void Reset() {
		bIsCrouched = bIsMoving = bIsWalking = bIsRunning = bIsJumping = bIsHanging = bIsFalling = false;
	}
};

class cLiveEntity : public cEntity {
	protected:
		// Movements
		float fHealth							= ZERO_FLOAT; // Value picked from ini file

		float fWalkSpeed						= ZERO_FLOAT; // Value picked from ini file
		float fRunSpeed							= ZERO_FLOAT; // Value picked from ini file
		float fCrouchSpeed						= ZERO_FLOAT; // Value picked from ini file
		float fClimbSpeed						= ZERO_FLOAT; // Value picked from ini file
		
		float fWalkJumpCoef						= ZERO_FLOAT; // Value picked from ini file
		float fRunJumpCoef						= ZERO_FLOAT; // Value picked from ini file
		float fCrouchJumpCoef					= ZERO_FLOAT; // Value picked from ini file
		
		float fWalkStamina						= ZERO_FLOAT; // Value picked from ini file
		float fRunStamina						= ZERO_FLOAT; // Value picked from ini file
		float fCrouchStamina					= ZERO_FLOAT; // Value picked from ini file

		float fJumpForce						= ZERO_FLOAT; // Value picked from ini file
		float fJumpStamina						= ZERO_FLOAT; // Value picked from ini file
		
		float fStaminaRestore					= ZERO_FLOAT; // Value picked from ini file
		float fStaminaRunMin					= ZERO_FLOAT; // Value picked from ini file
		float fStaminaJumpMin					= ZERO_FLOAT; // Value picked from ini file

		float fMaxItemsMass						= ZERO_FLOAT; // Value picked from ini file
		///

		// This variable control which physic to use on entity
		int iMotionType							= LIVE_ENTITY::Motion::None;
		int iPrevMotionType						= LIVE_ENTITY::Motion::None;

		// Hold states between physic updates
		Entity_States_Temp						States, PrevStates;

		// Var used for smooth movements of entity
		float				fMoveSmooth			= ZERO_FLOAT;
		float				fStrafeSmooth		= ZERO_FLOAT;
		float				fVerticalSmooth		= ZERO_FLOAT;

		float				fViewRange			= ZERO_FLOAT;

		// Used for bullet collision detection
		Leadwerks::Model *	pHitBox				= NULL;

		// Stamina always reach 1.0f
		float				fStamina			= ZERO_FLOAT;

		bool				bIsUnderSomething	= false;
		bool				bTiredness			= false;
		bool				bLanded				= true;
		bool				bHeavyFall			= false;

		Leadwerks::Model*	pFootsModel			= NULL;

		cInventory *		pInventory			= NULL;

		cEntity *			pFlashLight			= NULL;

		cFootstepManager *	pFootstepManager	= NULL;
	
	public:
		
		inline	void				SetHealth( float f )		{ fHealth = f; };
		inline	float				GetHealth()					{ return fHealth; };

		inline	void				SetStamina( float f )		{ fStamina = f; };
		inline	float				GetStamina()				{ return fStamina; };

		inline	void				SetViewRange( float f )		{ fViewRange = f; }
		inline	float				GetViewRange()				{ return fViewRange; }

				void				SetMotionType( int i );
		inline	int					GetMotionType( void )		{ return iMotionType; }
		inline	int					GetPrevMotionType( void )	{ return iPrevMotionType; }

		inline	bool				IsIdle()					{ return !HasState( LIVE_ENTITY::Actions::Moving );		}
		inline	bool				IsMoving()					{ return  HasState( LIVE_ENTITY::Actions::Moving );		}
		inline	bool				IsLeaning()					{ return  HasState( LIVE_ENTITY::Actions::Leaning );	}
		inline	bool				IsWalking()					{ return  HasState( LIVE_ENTITY::Actions::Walking );	}
		inline	bool				IsRunning()					{ return  HasState( LIVE_ENTITY::Actions::Running );	}
		inline	bool				IsJumping()					{ return  HasState( LIVE_ENTITY::Actions::Jumping );	}
		inline	bool				IsHanging()					{ return  HasState( LIVE_ENTITY::Actions::Hanging );	}
		inline	bool				IsFalling()					{ return  HasState( LIVE_ENTITY::Actions::Falling );	}
		inline	bool				IsCrouched()				{ return  HasState( LIVE_ENTITY::Actions::Crouched );	}

		inline	bool				IsPlainWalking()			{ return  ( iMotionType == LIVE_ENTITY::Motion::Walk	);	}
		inline	bool				IsFlying()					{ return  ( iMotionType == LIVE_ENTITY::Motion::Fly		);	}
		inline	bool				IsSwimming()				{ return  ( iMotionType == LIVE_ENTITY::Motion::Swim	);	}
		inline	bool				IsClimbing()				{ return  ( iMotionType == LIVE_ENTITY::Motion::P1ToP2	);	}

		inline	void				SetMoving  ( bool b )		{ SetState( LIVE_ENTITY::Actions::Moving,   b );	}
		inline	void				SetLeaning ( bool b )		{ SetState( LIVE_ENTITY::Actions::Leaning,  b );	}
		inline	void				SetWalking ( bool b )		{ SetState( LIVE_ENTITY::Actions::Walking,  b );	}
		inline	void				SetRunning ( bool b )		{ SetState( LIVE_ENTITY::Actions::Running,  b );	}
		inline	void				SetJumping ( bool b )		{ SetState( LIVE_ENTITY::Actions::Jumping,  b );	}
		inline	void				SetHanging ( bool b )		{ SetState( LIVE_ENTITY::Actions::Hanging,  b );	}
		inline	void				SetFalling ( bool b )		{ SetState( LIVE_ENTITY::Actions::Falling,  b );	}
		inline	void				SetCrouched( bool b )		{ SetState( LIVE_ENTITY::Actions::Crouched, b );	}

				void				SetPlainWalking();
				void				SetFlying();
				void				SetSwimming();
				void				SetClimbing();

		inline	bool				IsUnderSomething()			{ return bIsUnderSomething; }

		inline	void				SetWalkSpeed( float f )		{ fWalkSpeed = f;			}
		inline	float				GetWalkSpeed()				{ return fWalkSpeed;		}
		inline	void				SetRunSpeed( float f )		{ fRunSpeed = f;			}
		inline	float				GetRunSpeed()				{ return fRunSpeed;			}
		inline	void				SetCrouchSpeed( float f )	{ fCrouchSpeed = f;			}
		inline	float				GetCrouchSpeed()			{ return fCrouchSpeed;		}
		inline	void				SetJumpForce( float f )		{ fJumpForce = f;			}
		inline	float				GetJumpForce()				{ return fJumpForce;		}

		inline	void				SetMaxItemMass( float f )	{ fMaxItemsMass = f;		}
		inline	float				GetMaxItemMass()			{ return fMaxItemsMass;		}

		inline	cInventory *		GetInventory()				{ return pInventory;		}
		inline	cEntity *			GetFlashLight()				{ return pFlashLight;		}
		inline	cFootstepManager *	GetFootsetpManager()		{ return pFootstepManager;	}

		virtual void				Update()					= 0;
		virtual void				OnFrame()					= 0;

};