#ifndef STRUCTS_H
#define STRUCTS_H

#include "stdint.h"
#define uint unsigned int
typedef unsigned int long uint32;
#define int64 long long
#define uint64 unsigned long long
#define nint long long

struct AABB_t
{
	float fMin[3];
	float fMax[3];
};

enum AnimParamType_t: uint8_t
{

};

struct audio_source_indexstate_t
{
	unsigned int m_nPacketIndex;
	unsigned int m_nBufferSampleOffset;
	unsigned int m_nSampleFracOffset;
	unsigned int m_nDelaySamples;
};

struct AudioDeviceDesc
{
	bool m_bIsAvailable;
	bool m_bIsDefault;
};

struct BufferDesc_t
{
	int m_nElementCount;        // Number of vertices/indices
	int m_nElementSizeInBytes;  // Size of a single vertex/index
	unsigned long long m_pDebugName;           // Used to debug buffers
	unsigned long long m_pBudgetGroupName;
};

enum ButtonCode_t: int
{

};

struct Vector2D
{
	float x;
	float y;
};
struct Vector
{
	float x;
	float y;
	float z;
};
struct Vector4D
{
	float x;
	float y;
	float z;
	float w;
};

struct SboxVertex_t
{
	float x;
	float y;
	float z;
};

struct Color 
{
	float x;
	float y;
	float z;
};

struct Quaternion 
{
	float x;
	float y;
	float z;
	float w;
};

struct QAngle
{
	float x;
	float y;
	float z;
};

struct Rect_t
{
	float x;
	float y;
	float w;
	float h;
};

struct Rect3D_t
{
	float x;
	float y;
	float z;
	float w;
	float h;
	float l;
};



struct VMatrix
{
	Vector4D x;
	Vector4D y;
	Vector4D z;
	Vector4D w;
};
enum MaterialSystem2AppSystemDictFlags
{
	None = 0,
	IsConsoleApp = 1 << 0,
	IsGameApp = 1 << 1,
	IsDedicatedServer = 1 << 2,
	IsStandaloneGame = 1 << 3
};
struct MaterialSystem2AppSystemDictCreateInfo
{
	const char *pWindowTitle;
	MaterialSystem2AppSystemDictFlags iFlags;
	uint nSteamAppId;

	void *hInstance;
	void *hPrevInstance;
	int nCmdShow;
};

class CSfxTable;
class CAudioProcessor;
class HMaterialStrong;
class ISceneWorld;
class CRenderAttributes;
class SwapChainHandle_t {};
class HRenderTextureStrong;
class ISceneView;
class CTransformUnaligned {};
class CSceneObject;
class HModelStrong;
struct KeyValues3;
class CUtlString;
class MeshTraceInput {};
class MeshTraceOutput {};

template<typename A>
class CUtlVector;


class HAnimationGraphStrong;
class CPhysAggregateData;

struct VPhysXBodyPart_t;
struct VPhysXJoint_t;
class CPhysSurfaceProperties;

struct RnSphere_t {};
struct RnCapsule_t {};
struct RnHull_t;
struct RnMesh_t;

class CSamplerStateDesc {};
struct RenderBufferHandle_t {
	uint64 handle;
};
struct IndexBufferHandle_t {
	uint64 handle;
};
struct VertexBufferHandle_t {
	uint64 handle;
};
class HShaderStrong;
class HRenderTexture;
class CVfxComboIterator;
class CVfxProgramData;
class CVfxByteCodeManager;
struct VfxCompiledShaderInfo_t;

class HRenderMesh_InternalStrong;
class IAnimParameterInstance;
class IAnimParameterList;
class HSoundStrong;
class CSteamItemInstance;

class PhysicsTrace
{
public:
	enum Result {

	};
	enum Request {

	};
};

namespace fpxr
{
	class ApplicationConfig;
	class Compositor;
	class EventManager;
	class Input;
	class Instance;
	struct TextureSubmitInfo {};
	struct Matrix;
	class ViewInfo;
	class Event;
	class InputBooleanActionState;
	class InputFloatActionState;
	class InputVector2ActionState;
	class InputPoseActionState;
	class InputPoseHandState;
	struct VulkanInfo {};
	struct InstanceInfo {
		char szAppName[128];
		bool bUseDebugMessenger;
		int eGraphicsAPI;
		char szActionManifestPath[512];
	};
	struct InstanceProperties {};
}

class IPVS;
class IWorldReference;
class IPhysicsWorld;
class IPhysSurfacePropertyController;
struct SamplerStateHandle_t
{
	uint64 handle;
};
struct RenderDeviceInfo_t
{

};
struct TextureCreationConfig_t
{

};

class CTextureDesc;
class IRenderContext;
struct RenderShaderHandle_t
{
	uint64 handle;
}; 
struct SheetSequence_t; 
class HResourceManifest__;
struct SceneSystemPerFrameStats_t;
class ISceneLayer;
struct SceneVolumetricFogVolume_t {};
struct RenderViewport_t {};
class ITonemapSystem;
class IVolumetricFog;
class SteamHTMLSurface;
class ISteamHTMLSurface;
class ISteamUser;
class ISteamFriends;
class ISteamNetworkingMessages;
class ISteamNetworkingUtils;
class ISteamMatchmaking;
class ISteamGameServer;
class ISteamUtils;
class ISteamNetworkingSockets;
class ISteamApps;
class HSteamListenSocket {};
class HSteamNetPollGroup {};
class HSteamNetConnection {};
struct ovrLipSyncFrame {};
class YGNodeRef {};
class YGConfigRef {};
class YGValue {};
typedef void *YGMeasureFunc;
class CAnimVariant {};
class IPhysicsJoint;
class IPhysicsBody;
class IMaterial2;
class IMaterialNode;
class IMaterialMode;
class IPhysicsTrace;
class IPhysicsShape;
struct VertexLayout;

struct DisplayMode_t;
namespace Glue
{
	class Networking;
	class RenderDeviceManager;
	class Resources;
}
class HSceneViewRenderTarget {};
class HAuthTicket {};
struct SteamNetConnectionRealTimeStatus_t {};
struct SheetFrameImage_t {};

namespace Steam
{
	class Inventory;
	class Screenshots;
	namespace Ugc
	{
		class CUgcInstall;
		class CUgcQuery;
		class CUgcUpdate;
	}
}

struct SceneTonemapParameters_t;
struct SceneVolumetricFogParameters2_t {};

namespace NativeEngine
{
	class SDLGameController;
}

#endif
