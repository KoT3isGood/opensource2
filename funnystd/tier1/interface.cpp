#include "tier1/interface.h"
#include "tier1/utlvector.h"

CInterfaceRegistry *g_pInterfaceRegistries = NULL;
CInterfaceRegistry::CInterfaceRegistry( InstantiateInterfaceFn fn, const char *szName )
	: m_szName(szName)
{
	m_CreateFn = fn;
	m_pNext = g_pInterfaceRegistries;
	g_pInterfaceRegistries = this;
};

void *CreateInterface( const char *szName, int *pReturnCode )
{
	CInterfaceRegistry *pRegistry = g_pInterfaceRegistries;
	while (pRegistry)
	{
		if (!V_strcmp(szName, pRegistry->m_szName))
		{
			if (pReturnCode)
				*pReturnCode = 0;
			return pRegistry->m_CreateFn();
		}
		pRegistry = pRegistry->m_pNext;
	}
	if (pReturnCode)
		*pReturnCode = 1;
	return 0;
}
