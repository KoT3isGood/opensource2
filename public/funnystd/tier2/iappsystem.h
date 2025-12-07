#ifndef APP_SYSTEM_H
#define APP_SYSTEM_H

#include "tier1/interface.h"

//----------------------------------------------------------------------------
// App system is a module which has different applications based on the user's system.
// For example rendering could use either Vulkan or Metal based on the system.
//----------------------------------------------------------------------------
abstract_class IAppSystem
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
};

#endif
