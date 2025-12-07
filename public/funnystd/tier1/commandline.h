//================= Copyright kotofyt, All rights reserved ==================//
// Purpose: Command line handler for argc and argv. It also allows to find
// parameters and push your own.
//===========================================================================//

#ifndef TIER1_COMMANDLINE_H
#define TIER1_COMMANDLINE_H


#include "tier0/platform.h"

abstract_class ICommandLine
{
public:	
	virtual void CreateCommandLine( int argc, char **argv ) = 0;

	virtual bool CheckParam( const char *psz ) = 0;
	virtual char *ParamValue( const char* psz, const char *szDefaultValue = 0 ) = 0;

	virtual void AddParam( char *psz ) = 0;
	virtual void RemoveParam( char *psz ) = 0;

	virtual int ParamCount() = 0;
	virtual int FindParam( const char *psz ) = 0;
	virtual const char *GetParam(int nIndex) = 0;
};

ICommandLine *CommandLine();

#endif
