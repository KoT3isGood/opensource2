#include "helper.h"
#include "c.h"
#include "ld.h"
#include "tier1/utlstring.h"
#include "tier1/commandline.h"

CUtlVector<CUtlString> tier2_CompiledFiles = {
	"funnystd/tier2/fileformats/ini.cpp",
	"funnystd/tier2/fileformats/json.cpp",
	"funnystd/tier2/filesystem.cpp",
	"funnystd/tier2/filesystem_libc.cpp",
};
CUtlString tier2_lib;

DECLARE_BUILD_STAGE(tier2)
{
	CProject_t compileProject = {};
	LinkProject_t ldProject = {};

	compileProject.m_szName = "tier2";
	compileProject.files = tier2_CompiledFiles;
	compileProject.includeDirectories = all_IncludeDirectories;
	compileProject.bFPIC = true;
	compileProject.m_target.kernel = TARGET_KERNEL_WINDOWS_GNU;
	ldProject = ccompiler->Compile(&compileProject);
	ldProject.linkType = ELINK_STATIC_LIBRARY;

	CUtlString outputProject = linker->Link(&ldProject);
	tier2_lib = outputProject;

	return 0;
};
