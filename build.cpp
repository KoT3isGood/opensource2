#include "target.h"


CUtlVector<CUtlString> all_IncludeDirectories = {
	"public/funnystd"
};

#include "funnystd/tier0/__build.cpp"
#include "funnystd/tier1/__build.cpp"
#include "funnystd/tier2/__build.cpp"
#include "launcher/__build.cpp"
