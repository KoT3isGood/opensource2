#ifndef MURMUR2_H
#define MURMUR2_H
#include "stdint.h"
extern "C"
{
	uint32_t MurmurHash2        ( const void * key, int len, uint32_t seed );
	uint32_t MurmurHash2A       ( const void * key, int len, uint32_t seed );
	uint32_t MurmurHashNeutral2 ( const void * key, int len, uint32_t seed );
	uint32_t MurmurHashAligned2 ( const void * key, int len, uint32_t seed );
	uint64_t MurmurHash64A      ( const void * key, int len, uint64_t seed );
	uint64_t MurmurHash64B      ( const void * key, int len, uint64_t seed );
}

unsigned int MMHash2( const char *szKey, bool bLowerCase, unsigned int seed = 0x31415926 );

#endif
