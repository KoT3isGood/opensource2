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
		"launcher/launcher.cpp",
		"engine/engine2.cpp",
		"engine/time.cpp",
		"engine/testrender.cpp",
		"engine/murmur2.cpp",
		"engine/stringtoken.cpp",
		"external/murmur2/MurmurHash2.c",

		"server/maploader.cpp",
		"server/baseentity.cpp",
		"server/basemodelentity.cpp",
	};
	stCompile.includeDirectories = {
		"public",
		"external/murmur2",
		"external/cglm/include",
	};
	stCompile.m_target.kernel = TARGET_KERNEL_WINDOWS_GNU;
	stLink = ccompiler->Compile(&stCompile);
	stLink.linkType = ELINK_EXECUTABLE;
	szExeTempBin = linker->Link(&stLink);

	filesystem2->MakeDirectory("game");
	filesystem2->CopyFile("game", szExeTempBin);
	return 0;
};
