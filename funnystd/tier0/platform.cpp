#include "tier0/platform.h"
#include "tier0/lib.h"
#include "stdarg.h"
#include "unistd.h"
#include "sys/stat.h"
#include "dirent.h"
#include "time.h"
#include "signal.h"

#ifdef __linux__
#include "dlfcn.h"
#include "execinfo.h"
#endif
#ifdef __APPLE__
#include "dlfcn.h"
#endif
#ifdef __WIN32__
#include "windows.h"
#endif

PLATFORM_INTERFACE void Plat_FatalErrorFunc(const char* szFormat, ...)
{
	va_list list;
	va_start(list, szFormat);
	V_vprintf(szFormat, list);
	va_end(list);
	fflush(stdout);
#if defined(__WIN32__)
	__debugbreak();
#elif defined(__wasm32__)
#else
	raise(SIGTRAP);
#endif
	Plat_Exit(1);
}

PLATFORM_INTERFACE void Plat_ListDirRecursive(const char* szPath, ListDirCallbackFn file, ListDirCallbackFn dir)
{
	struct dirent *entry;
	DIR *dp = opendir(szPath);

	if (!dp) {
		return;
	}

	while ((entry = readdir(dp)) != NULL) {
		char full_path[1024];
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		snprintf(full_path, sizeof(full_path), "%s/%s", szPath, entry->d_name);

		struct stat statbuf;
		if (stat(full_path, &statbuf) == -1) {
			continue;
		}

		if (S_ISDIR(statbuf.st_mode)) {
			if (dir)
				dir(full_path);
			Plat_ListDirRecursive(full_path, file, dir);
		} else {
			if (file)
				file(full_path);
		}
	}

	closedir(dp);
}
PLATFORM_INTERFACE void Plat_ListDir(const char* szPath, ListDirCallbackFn file, ListDirCallbackFn dir)
{
	struct dirent *entry;
	DIR *dp = opendir(szPath);

	if (!dp) {
		return;
	}

	while ((entry = readdir(dp)) != NULL) {
		char full_path[1024];
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		snprintf(full_path, sizeof(full_path), "%s/%s", szPath, entry->d_name);

		struct stat statbuf;
		if (stat(full_path, &statbuf) == -1) {
			continue;
		}

		if (S_ISDIR(statbuf.st_mode)) {
			if (dir)
				dir(full_path);
		} else {
			if (file)
				file(full_path);
		}
	}

	closedir(dp);
}

PLATFORM_INTERFACE char *Plat_GetExtension( const char *szPath )
{
	char *last = (char*)szPath + V_strlen(szPath) - 1;
	while (last != szPath)
	{
		if (*last=='.')
		{
			break;
		};
		if (*last=='/')
		{
			return 0;
		}
		last--;
	}
	return last+1;
}

PLATFORM_INTERFACE void Plat_Backtrace( void )
{
#ifdef __linux__
	void *buffer[64];
	int nptrs = backtrace(buffer, 64);
	char **symbols = backtrace_symbols(buffer, nptrs);

	if (symbols == NULL) {
		V_printf("Backtrace failed\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < nptrs; i++) {
		V_printf(" [%d]   %s\n", i, symbols[i]);
	}

	free(symbols);
#endif
#ifdef __WIN32__
	/*
	void* buffer[64];
	USHORT nptrs = CaptureStackBackTrace(0, 64, buffer, NULL);

	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);

	SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256, 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	if (!symbol) {
		V_printf("Backtrace failed\n");
		return;
	}

	for (USHORT i = 0; i < nptrs; i++) {
		if (SymFromAddr(process, (DWORD64)(buffer[i]), 0, symbol)) {
			V_printf(" [%d] %s - 0x%0llX\n", i, symbol->Name, symbol->Address);
		} else {
			V_printf(" [%d] ??? - 0x%0llX\n", i, (DWORD64)(buffer[i]));
		}
	}

	free(symbol);
	*/
#endif
};

PLATFORM_INTERFACE void *Plat_LoadLibrary( const char *psz )
{
#ifdef __linux__
	void *lib = dlopen(psz, RTLD_GLOBAL | RTLD_NOW);
	if (!lib)
		V_printf("Failed to open %s\n\t%s\n", psz, dlerror());
	return lib;
#endif
#ifdef __APPLE__
	void *lib = dlopen(psz, RTLD_GLOBAL | RTLD_NOW);
	if (!lib)
		V_printf("Failed to open %s\n\t%s\n", psz, dlerror());
	return lib;
#endif
#ifdef __WIN32__
	return LoadLibraryA(psz);
#endif
}
PLATFORM_INTERFACE void *Plat_GetProc( void *lib, const char *psz )
{
#ifdef __linux__
	return dlsym(lib, psz);
#endif
#ifdef __APPLE__
	return dlsym(lib, psz);
#endif
#ifdef __WIN32__
	return (void*)GetProcAddress((HMODULE)lib, psz);
#endif
}
PLATFORM_INTERFACE void Plat_UnloadLibrary( void *lib )
{
#ifdef __linux__
	dlclose(lib);
#endif
#ifdef __WIN32__
	FreeLibrary((HMODULE)lib);
#endif
};

PLATFORM_INTERFACE double Plat_GetTime( void )
{
#ifndef __WIN32__
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	static time_t s_starttime = tp.tv_sec;

	return (tp.tv_sec-s_starttime)+tp.tv_nsec/1e9;
#else
	static LARGE_INTEGER s_startcount;
	static LARGE_INTEGER s_frequency;
	static int s_initialized = 0;

	LARGE_INTEGER tp;

	if (!s_initialized) {
		QueryPerformanceFrequency(&s_frequency);
		QueryPerformanceCounter(&s_startcount);
		s_initialized = 1;
	}

	QueryPerformanceCounter(&tp);

	return (double)(tp.QuadPart - s_startcount.QuadPart) / s_frequency.QuadPart;
#endif
}

PLATFORM_INTERFACE void Plat_Exit( int status )
{
#ifdef __linux__
	_exit(status);
#endif
#ifdef __APPLE__
	_exit(status);
#endif
#ifdef __WIN32__
	_exit(status);
#endif
};
