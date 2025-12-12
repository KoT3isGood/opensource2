//================= Copyright kotofyt, All rights reserved ==================//
// Purpose: Runs the engine and sets all the bindings.
//===========================================================================//


#include "engine/appsystem.h"
#include "tier0/platform.h"
#include "tier1/commandline.h"
#include "stdio.h"
#include "valvefunnystd.h"
#include "windows.h"
#include "libloaderapi.h"
#include "engine/structs.h"
#include "engine/engine2.h"
#include "engine/engine2vars.h"
#include "engine/testrender.h"
#include "engine/stringtoken.h"
#include "engine/inativecvar.h"
#include "maploader.h"
#include "stdlib.h"
#include "sourcetools/sourcetools.h"
#include "sourcetools/sourcetoolsvars.h"
#include "hammer/hammer.h"



CMaterialSystem2AppSystemDict *AppSystem;

fpxr::Instance *FPXRInstance;

// fix this bullshit

void CC RegisterEngineLogger( int id, const char *v )
{
	//printf("EngineLogger %s\n",v);
}
void CC OnClientOutput()
{
	pTestRendering->Frame(g_pEngineServiceMgr->GetEngineSwapChain());
}
void CC SetSystemInfo()
{

};

void CC RegisterNativeVar(void* pConVar)
{
	CNativeConsole::RegisterVar(pConVar);
}
void CC RegisterNativeCommand(void* pCommand)
{
	CNativeConsole::RegisterCommand(pCommand);
}
void CC OnConVarChanged(const char *szName, const char *szValue, const char *szPrevious)
{
	printf("changed %s\n",szName);
}

void CC OnConsoleCommand(const char *szCommand)
{
	printf("Command %s\n",szCommand);
}

struct SourceHandle_t
{
	void *pObject;
	unsigned int handle;
	struct SourceHandle_t *pNext;
};
SourceHandle_t *g_pCurrentSourceHandle = NULL;

int iLastHandleIndex = 0;
void *AcquireNextHandle(int hHandle)
{
	printf("Got %i\n", hHandle);
	g_pCurrentSourceHandle->handle = hHandle;
	return g_pCurrentSourceHandle->pObject;
}

void *GetByHandle(int hHandle)
{
	for (SourceHandle_t *pHandle = g_pCurrentSourceHandle; pHandle; pHandle = pHandle->pNext)
	{
		if (hHandle == pHandle->handle)
			return pHandle->pObject;
	}
	return NULL;
}
#define ALLOCATE_HANDLE( className ) \
if (type==StringToken(#className)) \
{ \
	SourceHandle_t *pHandle = new SourceHandle_t; \
	C##className *pSceneWorld = new C##className; \
	pSceneWorld->m_pSelf = ptr; \
	pHandle->pObject = pSceneWorld; \
	pHandle->pNext = g_pCurrentSourceHandle; \
	g_pCurrentSourceHandle = pHandle; \
	iLastHandleIndex++; \
	return iLastHandleIndex; \ 
}

#define ALLOCATE_HANDLE_INTERFACE( className ) \
if (type==StringToken(#className)) \
{ \
	SourceHandle_t *pHandle = new SourceHandle_t; \
	I##className *pSceneWorld = new I##className; \
	pSceneWorld->m_pSelf = ptr; \
	pHandle->pObject = pSceneWorld; \
	pHandle->pNext = g_pCurrentSourceHandle; \
	g_pCurrentSourceHandle = pHandle; \
	iLastHandleIndex++; \
	return iLastHandleIndex; \ 
}

int CC RegisterHandle( void *ptr, unsigned int type )
{
	ALLOCATE_HANDLE_INTERFACE(SceneWorld);
	ALLOCATE_HANDLE(SceneObject);
	ALLOCATE_HANDLE(SceneLightObject);
	ALLOCATE_HANDLE(SceneLightProbeVolumeObject);
	ALLOCATE_HANDLE(SceneSkyBoxObject);
	ALLOCATE_HANDLE(EnvMapSceneObject);
	return -1;
}

struct ManagedRenderSetup_t
{
	IRenderContext *renderContext;
	ISceneView *sceneView;
	ISceneLayer *sceneLayer;
	int colorImageFormat;
	int msaaLevel;
	SceneSystemPerFrameStats_t *stats;
};

void TiledCulling_Callback( ManagedRenderSetup_t _setup )
{
	CREATE_NATIVE(IRenderContext, renderContext);
	SET_NATIVE(renderContext, _setup.renderContext);
	CREATE_NATIVE(ISceneLayer, sceneLayer);
	SET_NATIVE(sceneLayer, _setup.sceneLayer);
	
	CREATE_NATIVE(ISceneView, sceneView);
	SET_NATIVE(sceneView, _setup.sceneView);

	g_pSceneSystem->RenderTiledLightCulling(_setup.renderContext, _setup.sceneView, sceneView->GetMainViewport());
};

void AddLayersToView(ISceneView *_pView, RenderViewport_t viewport, HSceneViewRenderTarget hColor, HSceneViewRenderTarget hDepth, uint64_t nMSAA, CRenderAttributes *_pRenderAttributes)
{
	FROM_NATIVE(ISceneView, pView);
	FROM_NATIVE(CRenderAttributes, pRenderAttributes);

	CREATE_NATIVE(CFrustum, pFrustum);
	SET_NATIVE(pFrustum, pView->GetFrustum());


	CREATE_NATIVE(ISceneLayer, pSceneLayer);
	SET_NATIVE(pSceneLayer, pView->AddRenderLayer("Lightbinner", viewport, 0, NULL));
	pSceneLayer->m_nLayerFlags = 0x10000 | 0x4;
	pSceneLayer->SetOutput(hColor, hDepth);
	pSceneLayer->AddObjectFlagsRequiredMask((uint64_t)SceneObjectFlags::IsLight);
	pSceneLayer->AddObjectFlagsExcludedMask(0x0);

	SET_NATIVE(pSceneLayer,
	pView->AddManagedProceduralLayer("Tiled Culling", viewport, (void*)TiledCulling_Callback, NULL, true)
	);
	pSceneLayer->m_nLayerFlags = 0x4 | 0x40 | 0x200 | 0x40000;
	pSceneLayer->SetOutput(hColor, hDepth);
	pSceneLayer->AddObjectFlagsRequiredMask(0x0);
	pSceneLayer->AddObjectFlagsExcludedMask(0x0);
}

void PipelineEnd(ISceneView *_pView, RenderViewport_t viewport, HSceneViewRenderTarget hColor, HSceneViewRenderTarget hDepth, uint64_t nMSAA, CRenderAttributes *_pRenderAttributes)
{
	
	CREATE_NATIVE(ISceneView, sceneView);
	SET_NATIVE(sceneView, _pView);
	
	CREATE_NATIVE(CFrustum, pFrustum);
	SET_NATIVE(pFrustum, sceneView->GetFrustum());

	FROM_NATIVE(CRenderAttributes, pRenderAttributes);
	FROM_NATIVE(ISceneView, pView);
}
enum LayerStage: int
{
	LAYER_STAGE_AfterDepthPrepass = 1000,
	LAYER_STAGE_AfterOpaque = 2000,
	LAYER_STAGE_AfterSkybox = 3000,
	LAYER_STAGE_AfterTransparent = 4000,
	LAYER_STAGE_AfterViewmodel = 5000,
	LAYER_STAGE_BeforePostProcess = 6000,
	LAYER_STAGE_Tonemapping = 6500,
	LAYER_STAGE_AfterPostProcess = 7000,
	LAYER_STAGE_AfterUI = 8000,
};

void OnLayer( LayerStage renderHookStage, ManagedRenderSetup_t _setup)
{
	CREATE_NATIVE(IRenderContext, renderContext);
	SET_NATIVE(renderContext, _setup.renderContext);
	CREATE_NATIVE(ISceneLayer, sceneLayer);
	SET_NATIVE(sceneLayer, _setup.sceneLayer);
	
	CREATE_NATIVE(ISceneView, sceneView);
	SET_NATIVE(sceneView, _setup.sceneView);
	
	CREATE_NATIVE(SceneSystemPerFrameStats_t, stats);
	SET_NATIVE(stats, _setup.stats);

	CREATE_NATIVE(CFrustum, pFrustum);
	SET_NATIVE(pFrustum, sceneView->GetFrustum());

}

void OnSceneViewSubmitted( ISceneView* view )
{
	CREATE_NATIVE(ISceneView, sceneView);
	SET_NATIVE(sceneView, view);
	
	CREATE_NATIVE(CFrustum, pFrustum);
	SET_NATIVE(pFrustum, sceneView->GetFrustum());
	// should the camera pos be that low in some of the stuff?
}

int InternalIsActive()
{
	return true;
}
int InternalWantsInit()
{
	printf("Do we want it? I think %i\n",FPXRInstance->HasHeadset());
	return false;
}

const char *GetVulkanInstanceExtensionsRequired()
{
	if (!FPXRInstance->HasHeadset()) return "";
	return NULL;
}

const char *GetVulkanDeviceExtensionsRequired()
{
	if (!FPXRInstance->HasHeadset()) return "";
	return NULL;
}

void OnWindowActive(int bIsActive)
{
	printf("Window is active %i\n", bIsActive);
}

void CC EnginePrint( int severitty, const char* logger, const char* message )
{
	printf("%s\n", message);
}

void CC EnvironmentExit(int nCode)
{
	printf("EnvironmentExit: %i\n", nCode);
}
void ServerInit();
void ServerFrame();

int main( int nArgc, char **argv )
{
	CommandLine()->CreateCommandLine(nArgc, argv);

	char path[MAX_PATH] = {};

	if (GetModuleFileNameA(NULL, path, MAX_PATH)) {
		for (int i = strnlen(path, MAX_PATH) - 1; i >= 0; i--) {
			if (path[i] == '\\') {
				path[i] = '\0';
				break;
			}
		}
		SetCurrentDirectoryA(path);
	}
	char w64path[MAX_PATH] = {};
	strcpy(w64path, path);
	strcat(w64path, "\\bin\\win64");
	
	const char *newFolder = w64path;

	// Add bin/win64 to PATH
	const char *currentPath = getenv("PATH");
	if (!currentPath) currentPath = "";
	size_t newLen = strlen(newFolder) + strlen(currentPath) + 7;
	char *updatedPath = (char*)malloc(newLen);
	if (!updatedPath) {
		perror("malloc failed");
		return 1;
	}
	*updatedPath = 0;
	strcat(updatedPath, "PATH=");
	strcat(updatedPath, newFolder);
	strcat(updatedPath, ";");
	strcat(updatedPath, currentPath);

	if (putenv(updatedPath) != 0) {
		perror("putenv failed");
		free(updatedPath);
		return 1;
	}

	for (int i = 0; i<80;i++)
	{
		g_engine_callbackFunctions[i] = (void*)(0x100+i);
	}
	/*
	for (int i = 0; i<417;i++)
	{
		g_tools_callbackFunctions[i] = (void*)(0x2234000+i);
	}
	for (int i = 0; i<61;i++)
	{
		g_hammer_callbackFunctions[i] = (void*)(0x323400+i);
	}
	*/
	g_engine_callbackFunctions[3] = (void*)RegisterNativeVar;
	g_engine_callbackFunctions[4] = (void*)RegisterNativeCommand;
	g_engine_callbackFunctions[5] = (void*)OnConVarChanged;
	g_engine_callbackFunctions[6] = (void*)RegisterEngineLogger;
	g_engine_callbackFunctions[7] = (void*)EnvironmentExit;
	g_engine_callbackFunctions[13] = (void*)OnWindowActive;
	g_engine_callbackFunctions[22] = (void*)OnConsoleCommand;
	g_engine_callbackFunctions[24] = (void*)SetSystemInfo;
	g_engine_callbackFunctions[25] = (void*)SetSystemInfo;
	g_engine_callbackFunctions[26] = (void*)EnginePrint;
	g_engine_callbackFunctions[28] = (void*)OnClientOutput;
	g_engine_callbackFunctions[29] = (void*)OnSceneViewSubmitted;
	g_engine_callbackFunctions[30] = (void*)OnLayer;
	g_engine_callbackFunctions[31] = (void*)RegisterHandle;
	g_engine_callbackFunctions[49] = (void*)AddLayersToView;	
	g_engine_callbackFunctions[50] = (void*)PipelineEnd;
	g_engine_callbackFunctions[56] = (void*)SetSystemInfo;
	g_engine_callbackFunctions[70] = (void*)InternalIsActive;
	g_engine_callbackFunctions[71] = (void*)InternalWantsInit;
	g_engine_callbackFunctions[75] = (void*)GetVulkanInstanceExtensionsRequired;
	g_engine_callbackFunctions[76] = (void*)GetVulkanDeviceExtensionsRequired;
	g_engine_callbackFunctions[78] = (void*)SetSystemInfo;

	LoadLibraryA("engine2.dll");

	g_pengineLibrary = LoadLibraryA("engine2.dll");
	if(!g_pengineLibrary)
		_exit(1);
	
	FnBindingsImportDelegate igen_engine = (FnBindingsImportDelegate)GetProcAddress( (HMODULE)g_pengineLibrary, "igen_engine" );
	if(!igen_engine)
		_exit(1);

	engineBindingsInitialize();
	/*
	toolsBindingsInitialize();
	hammerBindingsInitialize();
	*/

	LoadLibraryA("coreclr.dll");


	MaterialSystem2AppSystemDictCreateInfo stCreateInfo = {};
	stCreateInfo.iFlags = IsGameApp;
	stCreateInfo.pWindowTitle = "opensauce2";
	AppSystem = (CMaterialSystem2AppSystemDict*)new CMaterialSystem2AppSystemDict(stCreateInfo);
	AppSystem->SetInStandaloneApp();
	AppSystem->SetSteamAppId(480);

	fpxr::InstanceInfo fpxrInstanceInfo = {};
	fpxrInstanceInfo.eGraphicsAPI = 2;
	fpxrInstanceInfo.bUseDebugMessenger = true;

	int v = SourceEnginePreInit("engine2.exe", (CMaterialSystem2AppSystemDict*)AppSystem->m_pSelf);
	if (!v) printf("Failed to SourceEnginePreInit\n");


	_CUtlVector<_CUtlString> resources_unusable = _CUtlVector<_CUtlString>(8,8);
	CUtlVector<CUtlString> resources;

	g_pResourceSystem->GetAllCodeManifests(GN((&resources_unusable)));
	resources = ToFunnyVectorString(&resources_unusable);

	for (auto r: resources)
	{
		printf("r %s\n",r.GetString());
	}

	v = SourceEngineInit(GN(AppSystem));
	if (!v) printf("Failed to SourceEngineInit\n");

	/*
	bool bTools = CommandLine()->CheckParam("-tools");

	if (bTools)
	{
		g_pToolFramework2->Spin();
		ToolsStallMonitor_IndicateActivity();
	}
	*/





	pTestRendering->Init();
	ServerInit();
	
	float fCurrentTime = Plat_GetTime();
	float fPreviousTime = fCurrentTime;
	Plat_SetCurrentFrame(0);

	while(true)
	{
		fPreviousTime = fCurrentTime;
		fCurrentTime = Plat_GetTime();
		SteamGameServer_RunCallbacks();
		SteamAPI_RunCallbacks();

		ServerFrame();
		SourceEngineFrame(AppSystem, fCurrentTime, fPreviousTime);
		
	}

	return 0;
}

