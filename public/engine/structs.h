#ifndef STRUCTS_H
#define STRUCTS_H

#include "cglm/quat.h"
#include "cglm/types.h"
#include "stdint.h"
#include "cglm/euler.h"
#define uint unsigned int
typedef unsigned int long uint32;
typedef long long int64;
#define uint64 unsigned long long
#define nint long long

template <typename A, typename Parent>
class RWVariable
{
	private:
	Parent *m_parent;
	A(*m_ReadOp)(Parent*);
	void(*m_WriteOp)(Parent*, A);
	public:
	RWVariable(void* ReadOp, void* WriteOp, Parent *parent)
	{
		m_ReadOp = (A(*)(Parent*))ReadOp;
		m_WriteOp = (void(*)(Parent*, A))WriteOp;
		m_parent = parent;
	}
	inline RWVariable& operator = (const A& other)
	{
		m_WriteOp( (Parent*)m_parent->m_pSelf , other);
		return *this;
	}
	inline operator A()
	{
		return m_ReadOp( (Parent*)m_parent->m_pSelf );
	}
};
#define RW_VAR(type, name, parent, readop, writeop) RWVariable<type, parent> name{readop, writeop,this};


#define abstract_class class
extern unsigned short g_engine_sHash;

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

struct QAngle
{
	float x;
	float y;
	float z;
};
#include "stdio.h"

struct Quaternion 
{
	float x;
	float y;
	float z;
	float w;

	inline void FromQAngle(QAngle angle)
	{
		mat4 m;
		versor q;
		glm_euler((vec3){glm_rad(angle.x), glm_rad(angle.y), glm_rad(angle.z)}, m);
		glm_mat4_quat(m, q);
		x = q[0];
		y = q[1];
		z = q[2];
		w = q[3];
	};

	inline QAngle ToQAngle()
	{
		mat4 m;
		QAngle v;
		
		glm_quat_mat4((float*)this, m);
		glm_euler_angles(m, (float*)&v);

		return v;
	};

	inline Vector GetForwardAxis()
	{
		vec3 f = {0,0,1};
		vec3 f2 = {0,0,0};
		versor q = {x,y,z,w};
		glm_quat_rotatev(q, f, f2);

		return *(Vector*)f2;
	}
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
class IRayTraceSceneWorld;
class CRenderAttributes;
struct SwapChainHandle_t {
public:
	uint64_t nHandle;
};
class HRenderTextureStrong;
class ISceneView;
class CTransformUnaligned {
public:
	CTransformUnaligned()
	{
		m_position = (Vector){0,0,0};
		m_scale = (Vector){1,1,1};
		m_rotation = (Quaternion){0,0,1,0};
	}
	Vector m_position;
	Vector m_scale;
	Quaternion m_rotation;	
};
class CSceneObject;
class HModelStrong;
struct KeyValues3;
class MeshTraceInput {};
class MeshTraceOutput {};


template<typename A>
class _CUtlVector;

class _CUtlString;


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
struct RenderViewport_t {
	int m_nVersion = 1;
	int m_nTopLeftX;
	int m_nTopLeftY;
	int m_nWidth;
	int m_nHeight;
	float m_fMinZ = 0;
	float m_fMaxZ = 1;
	void Init(int nTopLeftX, int nTopLeftY, int nWidth, int nHeight, float fMinZ, float fMaxZ)
	{
		m_nTopLeftX = nTopLeftX;
		m_nTopLeftY = nTopLeftY;
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_fMinZ = fMinZ;
		m_fMaxZ = fMaxZ;
	}
};
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
class HSceneViewRenderTarget {
public:
	uint64_t hHandle;
};
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

class QWidget;
class QScrollBar;
class QObject;
class QChar;
class QPixmap;
class QMenu;
class QAction;
typedef Rect_t QRectF;
class QPushButton;
class QLabel;
class QStatusBar;
class QMenuBar;
class QToolBar;
class QLayout;
class CGameDataClass;
class CGameDataVariable;
class QGraphicsItem;
class GDIVItemSet;
class CHelperInfo;
class CGameData;
class QLineEdit;
class QScreen;
class QGraphicsView;
class QPainter;
class QPen;
class QStringList;
class QTextDocument;
class QMimeData;
class CMapNode;
class CSelection;
class ISelectionSet;
class IAsset;
class QTextCursor;
class CToolCamera;
class CWorkPlane;

enum EnumChildrenPos_t
{

};

namespace Trace
{

};

namespace NativeHammer
{
	class Options;
}
namespace Trace
{
	class TraceRequest
	{

	};
	class TraceResult
	{

	};
}

// Make it feel more C-like
enum class SceneObjectFlags :uint64_t 
{
	None = 0,
	IsOpaque = 0x0000000000000001,
	IsTranslucent = 0x0000000000000002,
	IsLight = 0x0000000000000004,
	IsSunLight = 0x0000000000000008,
	IsLightVolume = 0x0000000000000010,
	IsDecal = 0x0000000000000020,
	IsDynamicDecals = 0x0000000000000040,
	IsEnvMap = 0x0000000000000080,
	IsDirectLight = 0x0000000000000100,
	IsIndirectLight = 0x0000000000000200,

	//------------------------------------------------------------------------------
	// Rendering order: Logical layers we organize a scene into
	//------------------------------------------------------------------------------
	ViewModelLayer = 0x0000000000001000,   // Render only in viewmodel pass
	Skybox3DLayer = 0x0000000000002000,
	DisabledInLowQuality = 0x0000000000004000,
	IsHammerGeometry = 0x0000000000008000,
	EffectsBloomLayer = 0x0000000000010000,
	GameOverlayLayer = 0x0000000000020000,
	ExcludeGameLayer = 0x0000000000040000,
	UIOverlayLayer = 0x0000000000080000,

	//------------------------------------------------------------------------------
	// Hammer- / Tools-specific rendering order
	//------------------------------------------------------------------------------
	ToolsUnlitLayer = 0x0000000000100000,
	ToolSceneOverlayLayer = 0x0000000000200000,
	HammerPrefabStencilLayer = 0x0000000000400000,
	HammerSelectionStencilLayer = 0x0000000000800000,
	HammerEnabledStencilLayer = 0x0000000001000000,

	//------------------------------------------------------------------------------
	// Rendering properties of a scene object
	//------------------------------------------------------------------------------
	HasAOProxies = 0x0000000010000000,
	AlphaTestZPrepass = 0x0000000020000000,
	AddsDependentView = 0x0000000040000000, // Adds a dependent view, doesn't render itself (also safe to draw multiple times).  Hint to kick this work off early
	NeedsDynamicReflectionMap = 0x0000000080000000, // Meshes with this flag set will cause a dynamic reflection to launch and have it bound into the context
	Reflects = 0x0000000100000000,
	CastShadowsEnabled = 0x0000000200000000,
	DoesNotAcceptDecals = 0x0000000400000000,
	WantsFrameBufferCopyTexture = 0x0000000800000000,
	IssuesQueries = 0x0000001000000000,
	StaticObject = 0x0000002000000000, // Static, permanent part of the "world"
	EnvironmentMapped = 0x0000004000000000,
	MaterialSupportsShadows = 0x0000008000000000, // The material supports casting shadows, but the object will only cast shadows if MATERIAL_SUPPORTS_SHADOWS and CAST_SHADOWS_ENABLED are set
	NoZPrepass = 0x0000010000000000, // Opt out of z-prepass
	ForwardLayerOnly = 0x0000020000000000, // Render only in a forward layer  
	NoOcclusionCulling = 0x0000040000000000, // Don't occlusion cull this scene object 
	NoPVSCulling = 0x0000080000000000, // Don't pvs cull this scene object

	CastShadows = MaterialSupportsShadows | CastShadowsEnabled,

	/// <summary>
	/// Not rendered in cubemaps
	/// </summary>
	HideInCubemaps = 0x0010000000000000,
	ExecuteBefore = 0x0200000000000000,
	ExecuteAfter = 0x0400000000000000,

	NeedsLightProbe = 0x0000400000000000,


	IsLoaded = 0x2000000000000000
};

// Shortened get native
#define GN(var) (decltype(var))(var->m_pSelf)
#define SET_NATIVE(var, v) var->m_pSelf = (void*)(v)
#define CREATE_NATIVE(type, var) type __##var; type *var = &__##var
#define CREATE_NATIVE_ALLOC(type, var) type *var = new type
#define FROM_NATIVE(type, var) CREATE_NATIVE(type, var); SET_NATIVE(var, _##var) 
#define FROM_NATIVE_ALLOC(type, var) CREATE_NATIVE_ALLOC(type, var); SET_NATIVE(var, _##var) 

#endif
