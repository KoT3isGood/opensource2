#include "helper.h"
#include "c.h"
#include "ld.h"
#include "tier1/utlstring.h"
#include "tier1/commandline.h"

CUtlVector<CUtlString> tier0_CompiledFiles = {
	"funnystd/tier0/lib.cpp",
	"funnystd/tier0/mem.cpp",
	"funnystd/tier0/platform.cpp",
};

CUtlString tier0_lib;

DECLARE_BUILD_STAGE(tier0)
{
	CProject_t compileProject = {};
	LinkProject_t ldProject = {};

	compileProject.m_szName = "tier0";
	compileProject.files = tier0_CompiledFiles;
	compileProject.includeDirectories = all_IncludeDirectories;
	compileProject.bFPIC = true;
	compileProject.m_target.kernel = TARGET_KERNEL_WINDOWS_GNU;
	ldProject = ccompiler->Compile(&compileProject);
	ldProject.linkType = ELINK_STATIC_LIBRARY;

	CUtlString outputProject = linker->Link(&ldProject);
	
	tier0_lib = outputProject;

	return 0;
};
