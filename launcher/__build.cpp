//================= Copyright kotofyt, All rights reserved ==================//
// Purpose: Builds launcher, the engine and the game.
//
// At some point they will have to be built separately.
//===========================================================================//

#include "helper.h"
#include "target.h"
#include "c.h"
#include "ld.h"
#include "tier1/utlstring.h"

DECLARE_BUILD_STAGE(launcher)
{
	CProject_t stCompile;
	LinkProject_t stLink;
	CUtlString szExeTempBin;

	stCompile = {};
	stCompile.m_szName = "launcher";
	stCompile.files = {
		"external/murmur2/MurmurHash2.c",
		"launcher/launcher.cpp",

		"engine/engine2.cpp",
		"engine/time.cpp",
		"engine/testrender.cpp",
		"engine/valvefunnystd.cpp",
		"engine/murmur2.cpp",
		"engine/stringtoken.cpp",
		"engine/cvar.cpp",
		"engine/nativecvar.cpp",

		"server/game.cpp",
		"server/maploader.cpp",
		"server/entitysystem.cpp",
		"server/baseentity.cpp",
		"server/basemodelentity.cpp",
		"server/pointentity.cpp",
		"server/lightentity.cpp",
		"server/directionallight.cpp",
		"server/envsky.cpp",
		"server/envcubemap.cpp",
		"server/envcubemapbox.cpp",
		"server/envlightprobevolume.cpp",
		"server/envcombinedlightprobevolume.cpp",
	};
	stCompile.includeDirectories = {
		"public",
		"public/funnystd",
		"external/murmur2",
		"external/cglm/include",
	};
	stCompile.m_target.kernel = TARGET_KERNEL_WINDOWS_GNU;
	stLink = ccompiler->Compile(&stCompile);
	stLink.linkType = ELINK_EXECUTABLE;
	stLink.objects.AppendTail({tier0_lib});
	stLink.objects.AppendTail({tier1_lib});
	stLink.objects.AppendTail({tier2_lib});
	szExeTempBin = linker->Link(&stLink);

	filesystem2->MakeDirectory("game");
	filesystem2->CopyFile("game", szExeTempBin);
	return 0;
};
