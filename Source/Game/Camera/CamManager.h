
#pragma once

#include <Engine.h>

#include "Pointer.h"

#include "Effects\HeadBob.h"
#include "Effects\HeadMove.h"
#include "Effects\HeadLean.h"
#include "Effects\Zoom.h"

#include "Game\Entities\Entity.h"
#include "Game\Entities\Item\Usables\Flashlight.h"

namespace CAMERA_STATES {
	enum States {
		Firstpreson		= 1 << 0,
		Thirdperson		= 1 << 1,
		FlyCam			= 1 << 2,

		Attached		= ( Firstpreson | Thirdperson ),
		Free			= !Attached
	};
};

class cCamManager : public cStateClass, public cEntitySection/*, public cCallbackOnce */{
	private:
		Leadwerks::Camera *pCamera					= NULL;
		cEntity *pParent							= NULL;
		cEntity *pOldParent							= NULL;

		Leadwerks::Vec3 vCamRotation				= EMPTY_VEC3;
		Leadwerks::Vec3 vCamRelRotation				= EMPTY_VEC3;

		Leadwerks::Vec3 vCamPosition				= EMPTY_VEC3;
		Leadwerks::Vec3 vCamRelPosition				= EMPTY_VEC3;
		bool bRotating								= false;

		// Cam Value
		float fCamHeigth							= ZERO_FLOAT;
		float fCamInterpolation						= ZERO_FLOAT;
		float fCamShift								= 1.0;
		float fFlySpeed								= ZERO_FLOAT;
		float fFlyBoost								= ZERO_FLOAT;

		cPointer *pPointer							= NULL;
		cHeadBob *pHeadBob							= NULL;
		cHeadMove *pHeadMove						= NULL;
		cHeadLean *pHeadLean						= NULL;
		cZoom *pZoom								= NULL;

		bool bNightVision							= false;
		bool bThermalVision							= false;
		bool bFXAA									= false;
		
		float fMouseSensitivity						= ZERO_FLOAT;
		float fCamDifferenceX						= ZERO_FLOAT;
		float fCamDifferenceY						= ZERO_FLOAT;
		float fPointedDistance						= ZERO_FLOAT;

		bool bIsOK = false;
		bool bIsInitialized = false;

	public:
		cCamManager();
		~cCamManager();
		bool Initialize( cEntity *Parent );
		inline bool IsOK() { return bIsOK; }
		inline bool IsInitialized() { return bIsInitialized; }

	private:

		bool LoadPostEffect( std::string FilePath = "" );

	public:

		// Set a parent for the camera
		bool SetParent( cEntity *Entity );

		// Return actual parent
		inline cEntity *GetParent( void )				{ return pParent; }

		// Temporally remove actual parent, if there is one
		bool Detach( void );

		// Restore old parent, if there was one
		bool Attach( void );

		// Return true value if camera is rotating, otherwise false
		inline bool IsRotating( void )					{ return bRotating; }

		// Get Leadwerks camera object
		inline Leadwerks::Camera *GetCamera( void )		{ return pCamera; };

		// Return the camera height
		inline float GetStdHeight( void )				{ return fCamHeigth; };

		// Return distance between camera and pointed point
		inline float GetPointedDistance( void )			{ return fPointedDistance; }

		// Return a vector with the sum of all position modifier by all camera effects
		inline Leadwerks::Vec3 GetRelPos( void )		{ return vCamRelPosition; };

		// Return a vector with the sum of all rotation modifier by all camera effects
		inline Leadwerks::Vec3 GetRelDir( void )		{ return vCamRelRotation; };
		
		inline void			SetCameraFirstPerson()		{ OnlyState( CAMERA_STATES::Firstpreson ); SetState( CAMERA_STATES::Attached, true ); };
		inline void			SetCameraThirdPerson()		{ OnlyState( CAMERA_STATES::Thirdperson ); SetState( CAMERA_STATES::Attached, true ); };
		inline void			SetCameraFree()				{ OnlyState( CAMERA_STATES::Free );		   SetState( CAMERA_STATES::Free, true ); };

		inline bool			IsCameraAttached()			{ return !HasState( CAMERA_STATES::Attached ); };
		inline bool			IsCameraFree()				{ return !HasState( CAMERA_STATES::Free ); };

		inline cHeadBob *	GetHeadBobEffect( )			{ return pHeadBob; }
		inline cHeadMove *	GetHeadMoveEffect( )		{ return pHeadMove; }
		inline cHeadLean *	GetHeadLeanEffect( )		{ return pHeadLean; }
		inline cZoom *		GetZoomEffect( )			{ return pZoom; }

	private:
		// Physic
		void UpdateFreeMode( void );
		void UpdateRotation( void );
		void UpdatePosition( void );

	public:
		void Update( void );

};

extern cCamManager *pCamManager;

cCamManager *CamManager();
inline cCamManager *CamManager() { if ( !pCamManager ) pCamManager = new cCamManager(); return ( pCamManager ); }