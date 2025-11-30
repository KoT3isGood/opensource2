#include "murmur2.h"

unsigned int StringToken(const char *szString)
{
	return MMHash2(szString, true);
}
