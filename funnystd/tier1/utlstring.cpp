#include "tier1/utlstring.h"
#include "tier0/lib.h"
#include "stdarg.h"
#include "tier0/platform.h"
CUtlString::CUtlString( void )
{
	m_data.Reserve(1);
	m_data[0]=0;
}

CUtlString::CUtlString( const char *szFormat, ... )
{
	if (szFormat == 0)
	{
		m_data.Reserve(1);
		m_data[0]=0;
		return;
	}
	va_list vlArgs;
	va_start(vlArgs, szFormat);
	va_list vlArgs2;
	va_copy(vlArgs2, vlArgs);
	size_t nSize = V_vsnprintf(NULL, 0, szFormat, vlArgs2);
	m_data.Reserve(nSize + 1);
	va_end(vlArgs2);
	V_vsnprintf(m_data.GetData(), nSize + 1, szFormat, vlArgs);
	m_data.Resize(nSize + 1);
	va_end(vlArgs);
}

CUtlString::CUtlString( const CUtlString &sz )
{
	m_data = sz.m_data;
};

void CUtlString::AppendTail( const char *psz )
{
	m_data.Resize(V_strlen(m_data.GetData()));
	m_data.AppendTail(psz,V_strlen(psz));
	m_data.Reserve(m_data.GetSize()+1);
	m_data[m_data.GetSize()] = 0;
}
void CUtlString::AppendHead( const char *psz )
{
	m_data.AppendHead(psz,V_strlen(psz));
}

void CUtlString::AppendTail( char ch )
{
	m_data.Resize(V_strlen(m_data.GetData()));
	m_data.AppendTail(ch);
	m_data.Reserve(m_data.GetSize()+1);
	m_data[m_data.GetSize()] = 0;
}
void CUtlString::AppendHead( char ch )
{
	m_data.AppendHead(ch);
}
void CUtlString::AppendAt( size_t nPosition, const char *psz )
{

}

void CUtlString::RemoveTail( size_t nCount )
{

}
void CUtlString::RemoveHead( size_t nCount )
{

}
void CUtlString::RemoveAt( size_t nPosition, size_t nCount )
{

}
CUtlString CUtlString::GetExtension( void )
{
	return Plat_GetExtension(GetString());
}

char *CUtlString::GetString( void )
{
	return m_data.GetData();
}
size_t CUtlString::GetLenght( void )
{
	return V_strlen(GetString());
}

CUtlString::operator char*( void )
{
	return GetString();
}
CUtlString::operator CUtlVector<char>&( void )
{
	return m_data;
}

CUtlString &CUtlString::operator=(const CUtlString &sz)
{
	if (this != &sz)
	{
		m_data = sz.m_data;
	}
	return *this;
}

bool CUtlString::operator==(const char *psz)
{
	if (psz==0)
		psz = "";
	if (!V_strcmp(GetString(), psz))
		return true;
	return false;
}
bool CUtlString::operator!=(const char *psz)
{
	if (psz==0)
		psz = "";
	if (!V_strcmp(GetString(), psz))
		return false;
	return true;
}

bool CUtlString::operator==(CUtlString &string)
{
	if (!V_strcmp(GetString(), string.GetString()))
		return true;
	return false;
}
bool CUtlString::operator!=(CUtlString &string)
{
	if (!V_strcmp(GetString(), string.GetString()))
		return false;
	return true;
}
