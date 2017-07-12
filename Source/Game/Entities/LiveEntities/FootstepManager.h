
#pragma once

#include <Engine.h>
/*
	static const int PivotClass;//lua
	static const int ModelClass;//lua
	static const int CameraClass;//lua
	static const int DirectionalLightClass;//lua
	static const int SpotLightClass;//lua
	static const int PointLightClass;//lua
	static const int ListenerClass;//lua
	static const int Vec2Class;//lua
	static const int Vec3Class;//lua
	static const int Vec4Class;//lua
	static const int Mat4Class;//lua
	static const int SurfaceClass;//lua
	static const int BrushClass;//lua
	static const int EmitterClass;//lua
	static const int AttractorClass;//lua
	static const int JointClass;//lua
	static const int LensFlareClass;//lua
	static const int BoneClass;//lua
	static const int TerrainClass;//lua
	static const int TerrainPatchClass;//lua
	static const int BufferClass;//lua
	static const int ContextClass;//lua
	static const int ShaderClass;//lua
	static const int ComponentClass;//lua
	static const int TextureClass;//lua
	static const int MaterialClass;//lua
	static const int SpriteClass;//lua
	static const int DecalClass;//lua
	static const int VegetationLayerClass;//lua
	static const int ProbeClass;//lua
*/

enum {
	eGROUND_DEFAULT,
	eGROUND_ASPHALT,
	eGROUND_BRUSH,
	eGROUND_EARTH,
	eGROUND_FLOORTILE,
	eGROUND_GRAVEL,
	eGROUND_METAL,
	eGROUND_TIN,
	eGROUND_WATER,
	eGROUND_WOOD
};

class cFootstepManager {
	private:
		std::map < std::string, char >	mCounters;
		unsigned char iCounter			= 1;
		bool bIsOK						= false;
	
	public:
		cFootstepManager( std::string Section );
		~cFootstepManager( void );
		inline	bool IsOK() { return bIsOK; }

				void Step( Leadwerks::PickInfo &Info, bool bCourched = false, bool bRunning = false );



};