
#include "tier2/fileformats/ini.h"
#include "tier0/platform.h"
#include "tier1/utlstring.h"
#include "tier1/utlvector.h"
#include "tier0/lib.h"
#include "tier2/ifilesystem.h"

enum ESectionType
{
	SECTIONTYPE_STRING,
	SECTIONTYPE_BOOLEAN,
	SECTIONTYPE_FLOAT,
};


struct SectionData_t
{
	ESectionType m_eSectionType;
	CUtlString m_szKey;
	
	bool m_bData;
	float m_fData;
	CUtlString m_szData;
};

abstract_class CINISection: public IINISection
{
public:
	virtual bool GetBoolValue( const char *szKeyName ) override;
	virtual int GetIntValue( const char *szKeyName ) override;
	virtual const char *GetStringValue( const char *szKeyName ) override;
	virtual CUtlString GetUTLStringValue( const char *szKeyName ) override;

	CUtlVector<SectionData_t*> m_values;
	CUtlString m_szSectionName;
};

bool CINISection::GetBoolValue( const char *szKeyName )
{

}

int CINISection::GetIntValue( const char *szKeyName )
{

}

const char *CINISection::GetStringValue( const char *szKeyName )
{
	for (auto &value: m_values)
	{
		if (value->m_szKey == szKeyName)
			return value->m_szData;
	}
	return 0;
}

CUtlString CINISection::GetUTLStringValue( const char *szKeyName )
{

}

class CINIFile: public IINIFile
{
public:
	virtual IINISection *GetSection( const char *szSectionName ) override;

	CUtlVector<CINISection*> m_sections;
};

IINISection *CINIFile::GetSection( const char *szSectionName )
{
	for ( auto &section: m_sections )
	{
		if ( section->m_szSectionName == szSectionName )
			return (IINISection*)section;
	}
	return 0;
}


class CINIManager: public IINIManager
{
public:
	virtual IINIFile *ReadFile( const char *psz ) override;
	virtual IINIFile *ReadString( const char *psz ) override;
	virtual void ReleaseFile( IINIFile *pFile ) override;
	bool IsWordSymbol( char c );
	CUtlVector<CUtlString> TokenizeString();
};
IINIFile *CINIManager::ReadFile( const char *psz )
{
	IFileHandle *pFile;
	CUtlBuffer<char> data;
	
	pFile = filesystem->Open(psz, FILEMODE_READ);
	if (!pFile)
		return NULL;
	data = CUtlBuffer<char>(pFile->Size()+1);
	uint32_t nSize = pFile->Read(data.GetMemory(), pFile->Size());
	pFile->Close();

	return ReadString(data.GetMemory());
}

IINIFile *CINIManager::ReadString( const char *psz )
{
	CINIFile *pFile = new CINIFile;
	*pFile = {};
	bool bIsQuoted = false;
	bool bIsSlash = false;

	bool bIsVarName;
	bool bIsVarValue;
	bool bIsSectionName;

	CINISection *pCurrentSection = 0;
	SectionData_t *pCurrentSectionData = 0;
	size_t i = 0;

	char c;

	CUtlString szStringValue;

	CUtlVector<CUtlString> tokens;
	uint32_t nCurrentLine = 0;

	while (true)
	{
		c = psz[i];
		i++;
		if (c == '\0')
			break;
		if (c == '\n')
			nCurrentLine++;
		

		if (bIsQuoted)
		{
			if (bIsSlash)
			{
				bIsSlash = false;
				switch (c)
				{
				case '\\':
					szStringValue.AppendTail('\\');
					continue;
				case '"':
					szStringValue.AppendTail('"');
					continue;
				case 'n':
					szStringValue.AppendTail('\n');
					continue;
				case 't':
					szStringValue.AppendTail('\n');
					continue;
				case '\n':
					szStringValue.AppendTail('\n');
					continue;
				default:
					szStringValue.AppendTail(c);
					continue;
				}
			} else {
				switch (c)
				{

				case '\\':
					bIsSlash = true;
					continue;

				case '\n':
					V_printf("line %i: new line was found but the string wasn't finished, did you forget to place \" in the end of the line?\n", nCurrentLine);
				case '"':
					bIsQuoted = false;
					if (szStringValue == 0)
						continue;
					tokens.AppendTail(szStringValue);
					szStringValue = 0;
					continue;
				default:
					szStringValue.AppendTail(c);
					continue;
				}
			}
			
		} else {
			if (c == '"')
			{
				bIsQuoted = true;
			}

			if (IsWordSymbol(c))
			{
				szStringValue.AppendTail(c);
				continue;
			} else {
				if (szStringValue != 0)
					tokens.AppendTail(szStringValue);
				szStringValue = 0;
				if (V_isgraph(c) && c != '"')
					tokens.AppendTail(CUtlString("%c", c));
			}
		}
	};


	for ( int i = 0; i < tokens.GetSize(); i++ )
	{
		if (tokens[i] == "]")
		{
				V_printf("Error: unexpected ]\n");
				return pFile;
		}
		if (tokens[i] == "[")
		{
			i++;
			if (i>=tokens.GetSize())
			{
				V_printf("Error: Trailing section start\n");
				return pFile;
			}

			pCurrentSection = (CINISection*)pFile->GetSection(tokens[i]);
			if (!pCurrentSection)
			{
				pCurrentSection = new CINISection;
				*pCurrentSection = {};
				pFile->m_sections.AppendTail(pCurrentSection);
			}
			pCurrentSection->m_szSectionName = tokens[i];
			
			i++;
			if (i>=tokens.GetSize())
			{
				V_printf("Error: Trailing section name\n");
				return pFile;
			}
			if (tokens[i] != "]")
			{
				V_printf("Error: expected ]\n");
				return pFile;
			}
			continue;
		}
		if (!pCurrentSection)
		{
			V_printf("Error: section wasn't specified\n");
			return pFile;
		}
		if (tokens[i] == "=")
		{
			V_printf("Error: expected key\n");
			return pFile;
		}

		pCurrentSectionData = new SectionData_t;
		pCurrentSectionData->m_eSectionType = SECTIONTYPE_STRING;
		pCurrentSectionData->m_szKey = tokens[i];

		i++;
		if (i>=tokens.GetSize())
		{
			V_printf("Error: trailing key\n");
			delete pCurrentSectionData;
			return pFile;
		}
		if (tokens[i] != "=")
		{
			V_printf("Error: expected =\n");
			delete pCurrentSectionData;
			return pFile;
		}

		i++;

		if (i>=tokens.GetSize())
		{
			V_printf("Error: expected value\n");
			delete pCurrentSectionData;
			return pFile;
		}
		pCurrentSectionData->m_szData = tokens[i];

		pCurrentSection->m_values.AppendTail(pCurrentSectionData);
	}
	return (IINIFile*)pFile;
}

void CINIManager::ReleaseFile( IINIFile *pFile )
{
	delete (CINIFile*)pFile;
}

bool CINIManager::IsWordSymbol( char c )
{
	if (V_isalnum(c))
		return true;
	switch (c)
	{
	case '-':
	case '_':
	case '.':
		return true;
	default:
		return false;
	}
}

CUtlVector<CUtlString> TokenizeString();

IINIManager *INIManager()
{
	static CINIManager manager;
	return &manager;
}

