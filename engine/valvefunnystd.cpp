#include "valvefunnystd.h"
#include "engine/structs.h"

CUtlVector<CUtlString> ToFunnyVectorString(_CUtlVector<_CUtlString> *valveVector)
{
	CUtlVector<CUtlString> f;
	int n;
	int i;

	n = valveVector->Count();
	
	for ( int i = 0; i < n; i++)
	{
		f.AppendTail(valveVector->Element(i));
	}

	return f;
};
