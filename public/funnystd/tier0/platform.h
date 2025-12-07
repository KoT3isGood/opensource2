//================= Copyright kotofyt, All rights reserved ==================//
// Purpose: Multi-platform implementation of some of the functions which
// are provided by each OS differently.
//===========================================================================//

#ifndef TIER0_PLATFORM_H
#define TIER0_PLATFORM_H

#include "stdint.h"
#include "stddef.h"

#ifdef __WIN32__
#define  DLL_EXPORT extern "C"
#define  DLL_IMPORT extern "C"

#define  DLL_CLASS_EXPORT
#define  DLL_CLASS_IMPORT

#define  DLL_GLOBAL_EXPORT   extern
#define  DLL_GLOBAL_IMPORT   extern
#else
#define  DLL_EXPORT   extern "C" __attribute__ ((visibility("default")))
#define  DLL_IMPORT   extern "C"

#define  DLL_CLASS_EXPORT __attribute__ ((visibility("default")))
#define  DLL_CLASS_IMPORT

#define  DLL_GLOBAL_EXPORT   extern __attribute ((visibility("default")))
#define  DLL_GLOBAL_IMPORT   extern

#endif

#ifdef TIER0_STATIC

#define GLOBAL_USED __attribute__((used)) 

#else

#define GLOBAL_USED __attribute__((used))

#ifdef TIER0_IMPLEMENTATION
#define PLATFORM_INTERFACE DLL_EXPORT
#define PLATFORM_OVERLOAD DLL_GLOBAL_EXPORT
#define PLATFORM_CLASS DLL_CLASS_EXPORT
#else
#define PLATFORM_INTERFACE DLL_IMPORT
#define PLATFORM_OVERLOAD DLL_GLOBAL_IMPORT
#define PLATFORM_CLASS DLL_CLASS_IMPORT
#endif

#endif

#define abstract_class class

PLATFORM_INTERFACE void Plat_FatalErrorFunc( const char *szFormat, ... );

typedef void( *ListDirCallbackFn )( const char *szPath );
PLATFORM_INTERFACE void Plat_ListDirRecursive( const char *szPath, ListDirCallbackFn file, ListDirCallbackFn dir );
PLATFORM_INTERFACE void Plat_ListDir( const char *szPath, ListDirCallbackFn file, ListDirCallbackFn dir );
PLATFORM_INTERFACE char *Plat_GetExtension( const char *szPath );
PLATFORM_INTERFACE void Plat_TrapSignals( void (*pfn)() );
PLATFORM_INTERFACE void Plat_Backtrace( void );

PLATFORM_INTERFACE void *Plat_LoadLibrary( const char *psz );
PLATFORM_INTERFACE void *Plat_GetProc( void *lib, const char *psz );
PLATFORM_INTERFACE void Plat_UnloadLibrary( void *psz );

PLATFORM_INTERFACE double Plat_GetTime( void );
PLATFORM_INTERFACE void Plat_Exit( int status );

#endif
