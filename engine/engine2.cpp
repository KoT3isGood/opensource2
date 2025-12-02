#include "engine/engine2.h"

void *g_pengineLibrary;
bool g_bIsengineInitialized;
void *g_nativeFunctions[2745];
void *g_callbackFunctions[80];
short g_sHash = 0;


class g_pEngineServiceMgr *g_pEngineServiceMgr = new class g_pEngineServiceMgr;
class g_pRenderDevice *g_pRenderDevice = new class g_pRenderDevice;
class g_pMaterialSystem2 *g_pMaterialSystem2 = new class g_pMaterialSystem2;
class g_pMeshSystem *g_pMeshSystem = new class g_pMeshSystem;
class g_pSceneSystem *g_pSceneSystem = new class g_pSceneSystem;
class g_pWorldRendererMgr *g_pWorldRendererMgr = new class g_pWorldRendererMgr;
class g_pEnginePVSManager *g_pEnginePVSManager = new class g_pEnginePVSManager;
class EngineGlue *EngineGlue = new class EngineGlue;

class RenderTools *RenderTools = new class RenderTools;
