#include "windows.h"

double Plat_GetTime( void )
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
