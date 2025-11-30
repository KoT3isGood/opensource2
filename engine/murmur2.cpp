#include "murmur2.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"

unsigned int MMHash2( const char *szKey, bool bLowerCase, unsigned int seed )
{
	char *szLowerCase;
	unsigned int hash;
	char *c;
	if (bLowerCase)
	{
		szLowerCase = (char*)malloc(strlen(szKey)+1);
		strcpy(szLowerCase, szKey);
		for ( c = szLowerCase; *c; c++)
		{
			*c = tolower(*c);
		}
		hash = MurmurHash2(szLowerCase, strlen(szKey), seed);
		free(szLowerCase);
		return hash;
	}
	else
	{
		return MurmurHash2(szKey, strlen(szKey), seed);
	}
	return 0;
};
