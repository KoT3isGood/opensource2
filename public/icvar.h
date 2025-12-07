#ifndef CONSOLE_H
#define CONSOLE_H

#include "tier2/iappsystem.h"
#include "tier0/platform.h"
#include "tier1/utlstring.h"
#include "tier1/utlvector.h"

class ConVar;
class ConCommand;


typedef void(*ConCommandFn)(int argc, char **argv);

abstract_class IConsole: public IAppSystem
{
public:
	// Variables
	virtual void RegisterVar( ConVar *cvar ) = 0;
	virtual void UnRegisterVar( ConVar *cvar ) = 0;
	virtual ConVar *FindVar( const char *pName ) = 0;

	// Commands
	virtual void RegisterCommand( ConCommand *cvar ) = 0;
	virtual void UnRegisterCommand( ConCommand *cvar ) = 0;
	virtual ConCommand *FindCommand( const char *pName ) = 0;

	// Command buffer 
	virtual void Execute( void ) = 0;
	virtual void ExecuteArguments( CUtlVector<CUtlString> &args ) = 0;
	virtual CUtlVector<CUtlVector<CUtlString>> ParseCommandLine( CUtlString psz ) = 0;

	virtual void AddCommand( const char *psz ) = 0;
	virtual void InsertCommand( const char *psz ) = 0;
	
	CUtlVector<ConVar*> m_convars;
	CUtlVector<ConCommand*> m_commands;
};

IConsole *Console();


#define FCVAR_NONE 0
#define FCVAR_DEVELOPMENTONLY 0x1
#define FCVAR_GAMEDLL 0x2
#define FCVAR_CLIENTDLL 0x4
#define FCVAR_HIDDEN 0x8

#define FCVAR_PROTECTED 0x10
#define FCVAR_SPONLY 0x20
#define FCVAR_ARCHIVE 0x40
#define FCVAR_NOTIFY 0x80
#define FCVAR_CHEAT 0x100
#define FCVAR_REPLICATED 0x200

class ConVar
{
public:
	ConVar( const char *pName, const char *pDefaultValue, int flags );
	ConVar( const char *pName, const char *pDefaultValue, int flags,
		const char *pHelpString );
	ConVar( const char *pName, const char *pDefaultValue, int flags,
		const char *pHelpString, ConCommandFn callback );
	
	bool IsFlagSet( int flag );
	const char *GetHelpText( void );
	bool IsRegistered( void );
	const char *GetName( void );
	void AddFlags( int flags );
	bool IsCommand( void );

	void InstallChangeCallback( ConCommandFn );

	float GetFloat( void );
	int GetInt( void );
	bool GetBool( void );
	const char *GetString( void );

	void SetValue( const char *szValue );
	void SetValue( float fValue );
	void SetValue( int iValue );
private:
	CUtlString m_szName;
	CUtlString m_szHelpString;
	CUtlString m_szDefaultValue;

	CUtlString m_szValue;
	float m_fValue;
	int m_nValue;

	int m_flags;
};


class ConCommand
{
public:
	ConCommand(const char *pName, ConCommandFn callback, 
		const char *pHelpString=0, int flags=0);
	
	const char *GetName( void );
	const char *GetHelpText( void );
	ConCommandFn GetCallback( void );
private:
	CUtlString m_szName;
	CUtlString m_szHelpString = NULL;
	ConCommandFn m_callback;

	int m_flags;
};

#undef V_printf
#define V_printf(...) Msg(CUtlString(__VA_ARGS__).GetString())

void Msg( const char* message );
void Warning( const char* message );
void Error( const char* message );

#endif
