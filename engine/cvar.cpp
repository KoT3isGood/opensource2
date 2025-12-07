#include "icvar.h"
#include "tier2/ifilesystem.h"
#include "tier1/utlstring.h"
#include "tier1/utlvector.h"
#include "stdarg.h"
#include "engine/inativecvar.h"


void IConsole_NativeExec( const char *szCommand );
enum EConsoleMessageType
{
	CONSOLE_MESSAGE_TYPE_MESSAGE,
	CONSOLE_MESSAGE_TYPE_WARNING,
	CONSOLE_MESSAGE_TYPE_ERROR,
};

struct ConsoleMessage_t
{
	EConsoleMessageType type;
	CUtlString szMessage;
};

void Msg( const char* message )
{
	printf(message);

}

void Warning( const char* message )
{

}

void Error( const char* message )
{

}

class CConsole: public IConsole 
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	// Variables
	virtual void RegisterVar( ConVar *cvar ) override;
	virtual void UnRegisterVar( ConVar *cvar ) override;
	virtual ConVar *FindVar( const char *pName ) override;

	// Commands
	virtual void RegisterCommand( ConCommand *cvar ) override;
	virtual void UnRegisterCommand( ConCommand *cvar ) override;
	virtual ConCommand *FindCommand( const char *pName ) override;

	// Command buffer 
	virtual void Execute( void ) override;
	virtual void ExecuteArguments( CUtlVector<CUtlString> &args ) override;
	virtual CUtlVector<CUtlVector<CUtlString>> ParseCommandLine( CUtlString psz ) override;

	virtual void AddCommand( const char *psz ) override;
	virtual void InsertCommand( const char *psz ) override;
};

IConsole *Console()
{
	static CConsole s_console;
	return &s_console;
}

void CConsole::Init()
{
}

void CConsole::Shutdown()
{

}


void CConsole::RegisterVar( ConVar *cvar )
{
	m_convars.AppendTail(cvar);
}
void CConsole::UnRegisterVar( ConVar *cvar )
{

}
ConVar *CConsole::FindVar( const char *pName )
{
	for (auto &var: m_convars)
	{
		if (!V_strcmp(var->GetName(), pName))
			return var;
	}
	return NULL;
}

void CConsole::RegisterCommand( ConCommand *cvar )
{
	m_commands.AppendTail(cvar);
}
void CConsole::UnRegisterCommand( ConCommand *cvar )
{

}
ConCommand *CConsole::FindCommand( const char *pName )
{
	for (auto &var: m_commands)
	{
		if (!V_strcmp(var->GetName(), pName))
			return var;
	}
	return NULL;
}
CUtlString g_commandBuffer;

//-----------------------------------------------------------------------------
// Executes arguments from CConsole::Execute 
//-----------------------------------------------------------------------------
void CConsole::ExecuteArguments( CUtlVector<CUtlString> &args )
{
	if (args.GetSize()<1)
		return;

	ConCommand *cmd = CConsole::FindCommand(args[0]);
	if (cmd)
	{
		CUtlBuffer<char*> strbuffer(args.GetSize());
		for ( size_t i = 0; i < args.GetSize(); i++)
		{
			strbuffer[i] = args[i].GetString();
		};
		(cmd->GetCallback())(args.GetSize(), strbuffer);
	}
	CUtlString szArgs = args[0];
	for ( auto szArg: args )
	{
		szArgs.AppendTail(" ");
		szArgs.AppendTail(szArg);
	}
	if (CNativeConsole::RunCommand(args[0], szArgs))
		return;

	if (args.GetSize()!=2)
	{
		V_printf("not enough variable params\n");
		return;
	}

	ConVar *pVar = CConsole::FindVar(args[0]);
	if (pVar)
	{
		pVar->SetValue(args[1]);
	}

	if (CNativeConsole::SetStringValue(args[0], args[1]))
		return;

	V_printf("%s not found\n", args[0].GetString());
	return;
}

void CConsole::Execute( void )
{
	CUtlVector<CUtlVector<CUtlString>> commands = ParseCommandLine(g_commandBuffer);
	g_commandBuffer = 0;
	for (auto &command: commands)
	{
		ExecuteArguments(command);
	}
}

//-----------------------------------------------------------------------------
// Parses command buffer. 
//-----------------------------------------------------------------------------
CUtlVector<CUtlVector<CUtlString>> CConsole::ParseCommandLine( CUtlString psz )
{	
	CUtlVector<CUtlString> arguments;
	CUtlVector<CUtlVector<CUtlString>> commands;
	CUtlString szArgument;
	bool bIsQuote = false;
	for ( auto &c: (CUtlVector<char>&)psz )
	{
		if ( c == '\"' )
		{
			bIsQuote = !bIsQuote;
			continue;
		}

		if ( c == ';' || c == '\n' )
		{
			if (bIsQuote)
			{
				continue;
			}
			if (szArgument != 0)
				arguments.AppendTail(szArgument);
			if ( arguments.GetSize() > 0 )
				commands.AppendTail(arguments);
			szArgument = 0;
			arguments = {};
			continue;
		}
		if ( c == '\t' || c == ' ' )
		{
			if (bIsQuote)
			{
				szArgument.AppendTail(c);
				continue;
			}

			if (szArgument != 0)
				arguments.AppendTail(szArgument);
			szArgument = 0;
			continue;
		}
		szArgument.AppendTail(c);
	};
	if (szArgument != 0)
		arguments.AppendTail(szArgument);
	if ( arguments.GetSize() > 0 )
		commands.AppendTail(arguments);
	return commands;
}

void CConsole::AddCommand( const char *psz )
{
	g_commandBuffer.AppendTail(psz);
}

void CConsole::InsertCommand( const char *psz )
{
	g_commandBuffer.AppendHead(psz);
};

ConVar::ConVar( const char *pName, const char *pDefaultValue, int flags ) 
	: ConVar(pName, pDefaultValue, flags, 0)
{
		
}
ConVar::ConVar( const char *pName, const char *pDefaultValue, int flags,
	const char *pHelpString ) 
	: ConVar(pName, pDefaultValue, flags, pHelpString, 0)
{
}
ConVar::ConVar( const char *pName, const char *pDefaultValue, int flags,
	const char *pHelpString, ConCommandFn callback )
{
	m_szName = pName;
	m_flags = flags;
	m_szValue = pDefaultValue;
	m_fValue = V_atof(pDefaultValue);
	m_nValue = V_atoi(pDefaultValue);
	Console()->RegisterVar(this);
}

bool ConVar::IsFlagSet( int flag )
{

}
const char *ConVar::GetHelpText( void )
{
	return m_szHelpString;
}
bool ConVar::IsRegistered( void )
{

}
const char *ConVar::GetName( void )
{
	return m_szName;
}
void ConVar::AddFlags( int flags )
{

}
bool ConVar::IsCommand( void )
{

}

void ConVar::InstallChangeCallback( ConCommandFn )
{

}

float ConVar::GetFloat( void )
{
	return m_fValue; 
}
int ConVar::GetInt( void )
{
	return m_nValue; 
}
bool ConVar::GetBool( void )
{
	return m_nValue;
}
const char *ConVar::GetString( void ) 
{

}

void ConVar::SetValue( const char *szValue )
{
	if (!szValue)
		return;
	m_szValue = szValue;
	m_fValue = V_atof(szValue);
	m_nValue = V_atoi(szValue);
}
void ConVar::SetValue( float fValue )
{
	m_fValue = fValue;
	m_nValue = fValue;
	m_szValue = CUtlString("%f\n",fValue);
}
void ConVar::SetValue( int iValue )
{
	m_fValue = iValue;
	m_nValue = iValue;
	m_szValue = CUtlString("%i\n",iValue);
}

ConCommand::ConCommand(const char *pName, ConCommandFn callback, 
	const char *pHelpString, int flags)
{
	m_szName = pName;
	m_callback = callback;
	m_flags = flags;
	Console()->RegisterCommand(this);
};
const char *ConCommand::GetHelpText( void )
{
	return m_szHelpString;
}
const char *ConCommand::GetName( void )
{
	return m_szName;
}
ConCommandFn ConCommand::GetCallback( void )
{
	return m_callback;
}


void IConsole_Exec( int argc, char **argv )
{
	if (argc != 2)
		return;

	IFileHandle *f = filesystem->Open(argv[1], FILEMODE_READ);
	if (!f)
		return;
	CUtlBuffer<char> b(f->Size()+1);
	f->Read(b, b.GetSize());
	b[b.GetSize()-1] = 0;
	Console()->AddCommand(b);
	Console()->AddCommand(";");
	Console()->Execute();
}

ConCommand IConsole_ExecCmd("exec", IConsole_Exec);
