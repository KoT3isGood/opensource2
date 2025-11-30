//================= Copyright kotofyt, All rights reserved ==================//
// Copyright Facepunch Studios
//
// Purpose: Helper functions for compilers, filesystem2 and build stages.
//===========================================================================//


#include "engine/appsystem.h"
#include "stdio.h"
#include "windows.h"
#include "libloaderapi.h"
#include "engine/structs.h"
#include "engine/engine2.h"
#include "engine/engine2vars.h"
#include "engine/testrender.h"
#include "engine/stringtoken.h"
#include <cstdlib>

int iStructSizes[149]={24,1,16,2,24,4,0x11,4,4,3,4,8,0x14,0x1c,0x28,4,4,0x10,4,4,8,4,8,4,4,4,4,4,0x10,4,4,4,0xc,0xc,0x20,0x2dc,4,0x10,0x288,0x101,0x40,4,0x60,0x2c,0x20,4,4,4,4,8,8,4,4,4,4,8,4,4,1,4,4,0x28,0x28,0x88,0x78,4,4,0x28,4,4,4,4,4,4,0xc,0x10,0x10,0x18,4,4,4,8,4,1,4,0x30,4,8,4,4,8,4,4,1,0x1c,200,0x88,0x1c,0x10,1,4,4,4,8,0x4c,4,4,4,4,0x2c,4,0x1c,0x74,0x20,0x78,4,8,0x2c,4,4,4,4,0xc,8,0x10,8,0x14,4,4,0x40,4,1,0x18,4,8,4,4,4,4,4,4,8,8,4,4,4,8,4,};

CMaterialSystem2AppSystemDict *AppSystem;

fpxr::Instance *FPXRInstance;

// fix this bullshit

void CC RegisterEngineLogger( int id, const char *v )
{
	printf("EngineLogger %s\n",v);
}
static CCameraRenderer *s_pCameraRenderer;
void CC OnClientOutput()
{
	pTestRendering->Frame(g_pEngineServiceMgr->GetEngineSwapChain());
}
void CC SetSystemInfo()
{

};

void CC RegisterNativeVar(void* pConVar)
{
	ConVar pRealVar;
	pRealVar.m_pSelf = pConVar;
	printf("v   %s\n", pRealVar.GetName());
}
void CC RegisterNativeCommand(void* pCommand)
{
	ConCommand pRealCommand;
	pRealCommand.m_pSelf = pCommand;
	printf("c   %s\n", pRealCommand.GetName());
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
	g_pCurrentSourceHandle->handle = hHandle;
	return g_pCurrentSourceHandle->pObject;
}

int CC RegisterHandle( void *ptr, unsigned int type )
{
	printf("Created %i\n",type);
	if (type==StringToken("SceneWorld"))
	{
		SourceHandle_t *pHandle = new SourceHandle_t;
		ISceneWorld *pSceneWorld = new ISceneWorld;
		pSceneWorld->m_pSelf = ptr;
		pHandle->pObject = pSceneWorld;
		pHandle->pNext = g_pCurrentSourceHandle;
		g_pCurrentSourceHandle = pHandle;
		iLastHandleIndex++;
		return iLastHandleIndex; 
	}
	if (type==StringToken("SceneObject"))
	{
		SourceHandle_t *pHandle = new SourceHandle_t;
		CSceneObject *pSceneObject = new CSceneObject;
		pSceneObject->m_pSelf = ptr;
		pHandle->pObject = pSceneObject;
		pHandle->pNext = g_pCurrentSourceHandle;
		g_pCurrentSourceHandle = pHandle;
		iLastHandleIndex++;
		return iLastHandleIndex; 
	}
	return -1;
}

void AddLayersToView(ISceneView *_pView, RenderViewport_t viewport, HSceneViewRenderTarget hColor, HSceneViewRenderTarget hDepth, uint64_t nMSAA, CRenderAttributes *_pRenderAttributes)
{
	FROM_NATIVE(CRenderAttributes, pRenderAttributes);
	printf("n %p\n",_pRenderAttributes);
	printf("w %p\n",pRenderAttributes);
	printf("%i\n",pRenderAttributes->IsEmpty());
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

int main( int nArgc, char **argv )
{

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
		g_callbackFunctions[i] = (void*)(0x9900+i);
	}
	g_callbackFunctions[3] = (void*)RegisterNativeVar;
	g_callbackFunctions[4] = (void*)RegisterNativeCommand;
	g_callbackFunctions[5] = (void*)OnConVarChanged;
	g_callbackFunctions[6] = (void*)RegisterEngineLogger;
	g_callbackFunctions[7] = (void*)EnvironmentExit;
	g_callbackFunctions[13] = (void*)OnWindowActive;
	g_callbackFunctions[22] = (void*)OnConsoleCommand;
	g_callbackFunctions[24] = (void*)SetSystemInfo;
	g_callbackFunctions[25] = (void*)SetSystemInfo;
	g_callbackFunctions[26] = (void*)EnginePrint;
	g_callbackFunctions[28] = (void*)OnClientOutput;
	g_callbackFunctions[31] = (void*)RegisterHandle;
	g_callbackFunctions[49] = (void*)AddLayersToView; // https://github.com/Facepunch/sbox-public/blob/16809db7875c3a9bd405b6482a3c5c7d7038cb3e/engine/Sandbox.Engine/Systems/Render/RenderPipeline/RenderPipeline.cs#L23
	g_callbackFunctions[50] = (void*)SetSystemInfo; // 
	g_callbackFunctions[56] = (void*)SetSystemInfo;
	g_callbackFunctions[70] = (void*)InternalIsActive;
	g_callbackFunctions[71] = (void*)InternalWantsInit;
	g_callbackFunctions[75] = (void*)GetVulkanInstanceExtensionsRequired;
	g_callbackFunctions[76] = (void*)GetVulkanDeviceExtensionsRequired;
	g_callbackFunctions[78] = (void*)SetSystemInfo;

	printf("Lucky\n");
	engineBindingsInitialize();
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

	printf("STEAM APP ID %i\n",AppSystem->GetSteamAppId());
	int v = SourceEnginePreInit("engine2.exe", (CMaterialSystem2AppSystemDict*)AppSystem->m_pSelf);
	if (!v) printf("Failed to SourceEnginePreInit\n");
	v = SourceEngineInit(GN(AppSystem));
	if (!v) printf("Failed to SourceEngineInit\n");

	pTestRendering->Init();

	Plat_SetCurrentFrame(0);

	ISceneWorld *pSceneWorld = new ISceneWorld();

	while(true)
	{
		SteamGameServer_RunCallbacks();
		SteamAPI_RunCallbacks();
		SourceEngineFrame(AppSystem, 0, 0);
	}

	return 0;
}

