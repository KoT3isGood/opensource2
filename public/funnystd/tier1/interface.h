#ifndef INTERFACE_H
#define INTERFACE_H

#include "tier0/platform.h"

typedef void *( *CreateInterfaceFn )( const char *szName, int *pReturnCode );
typedef void *( *InstantiateInterfaceFn )( void );

class CInterfaceRegistry
{
public:
	CInterfaceRegistry( InstantiateInterfaceFn fn, const char *szName );

	InstantiateInterfaceFn m_CreateFn;
	const char *m_szName;

	CInterfaceRegistry *m_pNext;
};


#define EXPOSE_INTERFACE( className, interfaceName, versionName ) \
	static void *__Create##className##_interface() { return ( interfaceName* )( new className ); }; \
	static CInterfaceRegistry __Create##className##_registry( __Create##className##_interface, versionName );


void *CreateInterface( const char *szName, int *pReturnCode );

#endif
