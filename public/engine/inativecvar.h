#ifndef NATIVE_CONSOLE_H
#define NATIVE_CONSOLE_H

class CNativeConsole
{
public:
	static void RegisterVar( void *pVar );
	static void RegisterCommand( void *pVar );

	static bool RunCommand( const char *szCommandName, const char *szCommand );
	static bool SetStringValue( const char *szVarName, const char *szVarValue );
	static const char *GetStringValue( const char *szVarName );
	static bool SetFloatValue( const char *szVarName, float fVarValue );
	static float GetFloatValue( const char *szVarName );
	static bool SetIntValue( const char *szVarName, int iVarValue );
	static float GetIntValue( const char *szVarName );
};

#endif
