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
#include "engine/engine2.h"
#include <cstdlib>

int iStructSizes[149]={24,1,16,2,24,4,0x11,4,4,3,4,8,0x14,0x1c,0x28,4,4,0x10,4,4,8,4,8,4,4,4,4,4,0x10,4,4,4,0xc,0xc,0x20,0x2dc,4,0x10,0x288,0x101,0x40,4,0x60,0x2c,0x20,4,4,4,4,8,8,4,4,4,4,8,4,4,1,4,4,0x28,0x28,0x88,0x78,4,4,0x28,4,4,4,4,4,4,0xc,0x10,0x10,0x18,4,4,4,8,4,1,4,0x30,4,8,4,4,8,4,4,1,0x1c,200,0x88,0x1c,0x10,1,4,4,4,8,0x4c,4,4,4,4,0x2c,4,0x1c,0x74,0x20,0x78,4,8,0x2c,4,4,4,4,0xc,8,0x10,8,0x14,4,4,0x40,4,1,0x18,4,8,4,4,4,4,4,4,8,8,4,4,4,8,4,};

CMaterialSystem2AppSystemDict *AppSystem;

fpxr::Instance *FPXRInstance;

// fix this bullshit
class g_pEngineServiceMgr EngineServiceMgr;

void CC RegisterEngineLogger( int id, const char *v )
{
	printf("EngineLogger %s\n",v);
}
void CC OnClientOutput()
{
	int x;
	int y;
	EngineServiceMgr.GetEngineSwapChainSize(&x, &y);
	printf("%ix%i\n",x,y);
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
	printf("-------------------------------GetVulkanInstanceExtensionsRequired\n");
	printf("%s\n",FPXRInstance->GetRequiredInstanceExtensions());
	if (!FPXRInstance->HasHeadset()) return "";
	return FPXRInstance->GetRequiredInstanceExtensions();
}

const char *GetVulkanDeviceExtensionsRequired()
{
	printf("%s\n",FPXRInstance->GetRequiredInstanceExtensions());
	if (!FPXRInstance->HasHeadset()) return "";
	return FPXRInstance->GetRequiredDeviceExtensions();
}

void OnWindowActive(int bIsActive)
{
	printf("Window is active %i\n", bIsActive);
}

void CC EnginePrint( int severitty, const char* logger, const char* message )
{
	printf("%s\n", message);
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
		g_callbackFunctions[i] = (void*)(0x123400+i);
	}
	g_callbackFunctions[3] = (void*)RegisterNativeVar;
	g_callbackFunctions[4] = (void*)RegisterNativeCommand;
	g_callbackFunctions[5] = (void*)OnConVarChanged;
	g_callbackFunctions[6] = (void*)RegisterEngineLogger;
	g_callbackFunctions[13] = (void*)OnWindowActive;
	g_callbackFunctions[22] = (void*)OnConsoleCommand;
	g_callbackFunctions[24] = (void*)SetSystemInfo;
	g_callbackFunctions[25] = (void*)SetSystemInfo;
	g_callbackFunctions[26] = (void*)EnginePrint;
	g_callbackFunctions[28] = (void*)OnClientOutput;
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
	FPXRInstance = new fpxr::Instance(fpxrInstanceInfo);

	printf("STEAM APP ID %i\n",AppSystem->GetSteamAppId());
	int v = SourceEnginePreInit("engine2.exe", (CMaterialSystem2AppSystemDict*)AppSystem->m_pSelf);
	if (!v) printf("Failed to SourceEnginePreInit\n");
	v = SourceEngineInit((CMaterialSystem2AppSystemDict*)AppSystem->m_pSelf);
	if (!v) printf("Failed to SourceEngineInit\n");

	Plat_SetCurrentFrame(0);

	while(true)
	{
		SteamGameServer_RunCallbacks();
		SteamAPI_RunCallbacks();
		SourceEngineFrame(AppSystem, 0, 0);
	}

	return 0;
}

class CGameAppSystem: CAppSystem
{
public:
	virtual void Init() override;
};
