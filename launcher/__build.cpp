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
		"engine/testrender.cpp",
	};
	stCompile.includeDirectories = {
		"public"
	};
	stCompile.m_target.kernel = TARGET_KERNEL_WINDOWS_GNU;
	stLink = ccompiler->Compile(&stCompile);
	stLink.linkType = ELINK_EXECUTABLE;
	szExeTempBin = linker->Link(&stLink);

	filesystem2->MakeDirectory("game");
	filesystem2->CopyFile("game", szExeTempBin);
	return 0;
};
