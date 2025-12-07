#include "engine/engine2.h"
#include "engine/inativecvar.h"
#include "engine/structs.h"
#include "stdio.h"

struct NativeCommand_t
{
	_ConCommand *m_pCommand;
	struct NativeCommand_t *m_pNext;
};

static NativeCommand_t *g_pNativeCommands;

struct NativeConVar_t
{
	_ConVar *m_pConVar;
	struct NativeConVar_t *m_pNext;
};

static NativeConVar_t *g_pNativeConVars;

_ConCommand *CNativeConsole_FindCommand( const char *szName )
{
	NativeCommand_t *pCommand;
	for ( pCommand = g_pNativeCommands; pCommand; pCommand = pCommand->m_pNext)
	{
		if (!strcmp(pCommand->m_pCommand->GetName(), szName))
			return pCommand->m_pCommand;
	}
	return NULL;
}

_ConVar *CNativeConsole_FindVar( const char *szName )
{
	NativeConVar_t *pVar;
	for ( pVar = g_pNativeConVars; pVar; pVar = pVar->m_pNext)
	{
		if (!strcmp(pVar->m_pConVar->GetName(), szName))
			return pVar->m_pConVar;
	}
	return NULL;
}

void CNativeConsole::RegisterVar( void *_pVar )
{
	FROM_NATIVE_ALLOC(_ConVar, pVar);


	NativeConVar_t *pNativeConVar = new NativeConVar_t;
	pNativeConVar->m_pConVar = pVar;
	pNativeConVar->m_pNext = g_pNativeConVars;
	g_pNativeConVars = pNativeConVar;
}

void CNativeConsole::RegisterCommand( void *_pCommand )
{
	FROM_NATIVE_ALLOC(_ConCommand, pCommand);

	NativeCommand_t *pNativeCommand = new NativeCommand_t;
	pNativeCommand->m_pCommand = pCommand;
	pNativeCommand->m_pNext = g_pNativeCommands;
	g_pNativeCommands = pNativeCommand;
}

bool CNativeConsole::RunCommand( const char *szCommandName, const char *szCommand )
{
	_ConCommand *pCommand = CNativeConsole_FindCommand(szCommandName);
	if (!pCommand)
		return false;
	pCommand->Run(szCommandName);
	return true;
}

bool CNativeConsole::SetStringValue( const char *szVarName, const char *szVarValue )
{
	_ConVar *pVar = CNativeConsole_FindVar(szVarName);
	if (!pVar)
		return false;
	pVar->SetValue(szVarValue);
	return true;
}

const char *CNativeConsole::GetStringValue( const char *szVarName )
{

}
