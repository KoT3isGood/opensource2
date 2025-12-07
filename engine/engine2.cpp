#include "engine/engine2.h"
#include "sourcetools/sourcetools.h"

void *g_pengineLibrary;
bool g_bIsengineInitialized;
void *g_engine_nativeFunctions[2747];
void *g_engine_callbackFunctions[79];
void *g_ptoolsLibrary;
bool g_bIstoolsInitialized;
void *g_tools_nativeFunctions[4350];
void *g_tools_callbackFunctions[417];
void *g_phammerLibrary;
bool g_bIshammerInitialized;
void *g_hammer_nativeFunctions[616];
void *g_hammer_callbackFunctions[61];
int g_engine_iStructSizes[148]={24,1,16,2,24,4,0x11,4,4,3,4,8,0x14,0x1c,0x28,4,4,0x10,4,4,8,4,8,4,4,4,4,4,0x10,4,4,4,0xc,0xc,0x20,0x2dc,4,0x10,0x288,0x101,0x40,4,0x60,0x2c,0x20,4,4,4,4,8,8,4,4,4,4,8,4,4,1,4,4,0x28,0x28,0x88,0x78,4,4,0x28,4,4,4,4,4,4,0xc,0x10,0x10,0x18,4,4,4,8,4,1,4,0x30,4,8,4,4,8,4,4,1,0x1c,200,0x88,0x1c,0x10,1,4,4,4,8,0x4c,4,4,4,4,0x2c,4,0x1c,0x74,0x20,0x78,4,8,0x2c,4,4,4,4,0xc,8,0x10,8,0x14,4,4,0x40,4,1,0x18,4,8,4,4,4,4,4,4,8,8,4,4,4,8,4};
int g_hammer_iStructSizes[143]={4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
int g_tools_iStructSizes[129]={4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};

class g_pEngineServiceMgr *g_pEngineServiceMgr = new class g_pEngineServiceMgr;
class g_pRenderDevice *g_pRenderDevice = new class g_pRenderDevice;
class g_pMaterialSystem2 *g_pMaterialSystem2 = new class g_pMaterialSystem2;
class g_pMeshSystem *g_pMeshSystem = new class g_pMeshSystem;
class g_pSceneSystem *g_pSceneSystem = new class g_pSceneSystem;
class g_pWorldRendererMgr *g_pWorldRendererMgr = new class g_pWorldRendererMgr;
class g_pEnginePVSManager *g_pEnginePVSManager = new class g_pEnginePVSManager;
class EngineGlue *EngineGlue = new class EngineGlue;
class g_pToolFramework2 *g_pToolFramework2 = new class g_pToolFramework2;

class RenderTools *RenderTools = new class RenderTools;
