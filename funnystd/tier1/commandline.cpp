#include "tier1/commandline.h"
#include "tier1/utlvector.h"


class CCommandLine : public ICommandLine
{
public:	
	virtual void CreateCommandLine( int argc, char **argv ) override;

	virtual bool CheckParam( const char *psz ) override;
	virtual char *ParamValue( const char* psz, const char *szDefaultValue = 0 ) override;

	virtual void AddParam( char *psz ) override;
	virtual void RemoveParam( char *psz ) override;

	virtual int ParamCount() override;
	virtual int FindParam( const char *psz ) override;
	virtual const char *GetParam(int nIndex) override;
private:
	CUtlVector<char*> m_params;
};


void CCommandLine::CreateCommandLine( int argc, char **argv )
{
	m_params.AppendTail(argv,argc);
}

bool CCommandLine::CheckParam( const char *psz )
{
	for (auto szParam: m_params) {
		if (!V_strcmp(szParam, psz))
		{
			return true;
		}
	}
	return false;
}

char *CCommandLine::ParamValue( const char *psz, const char *szDefaultValue )
{
	int i = 0;
	for (auto szParam: m_params) {
		i++;
		if (i>=m_params.GetSize())
			break;
		if (!V_strcmp(szParam, psz))
			return m_params[i];
	}
	return (char*)szDefaultValue;
}

void CCommandLine::AddParam( char *psz )
{
	m_params.AppendTail(psz);
}

void CCommandLine::RemoveParam( char *psz )
{

}
int CCommandLine::ParamCount()
{
	return m_params.GetSize();
}
int CCommandLine::FindParam( const char *psz )
{
	int i = 0;
	for (auto szParam: m_params) {
		if (!V_strcmp(szParam, psz))
			return i;
		i++;
	}
	return 0;
}
const char *CCommandLine::GetParam(int nIndex)
{
	return m_params[nIndex];
}

ICommandLine *CommandLine()
{
	static CCommandLine s_CommandLine;
	return &s_CommandLine;
}
