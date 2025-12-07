//================= Copyright kotofyt, All rights reserved ==================//
// Purpose: A memory allocator for C and C++.
// For now it uses libc but it is possible to use own allocators
//===========================================================================//

#ifndef TIER0_MEM_H
#define TIER0_MEM_H

#include "platform.h"
#include "lib.h"

PLATFORM_INTERFACE void *V_malloc( int nSize );
PLATFORM_INTERFACE void V_free( void *pMem );
PLATFORM_INTERFACE void *V_realloc( void *pMem, int nSize );

void *operator new( size_t nCount );
void *operator new ( size_t nCount, void *pPtr );
void operator delete( void *pMem ) noexcept;

#endif
