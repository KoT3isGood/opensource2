//================= Copyright kotofyt, All rights reserved ==================//
// Purpose: Own implementation of string.
//===========================================================================//

#ifndef TIER1_UTL_STRING_H
#define TIER1_UTL_STRING_H

#include "tier1/utlvector.h"

class CUtlString {
public:
	CUtlString( void );	
	CUtlString( const char *psz, ... );
	CUtlString( const CUtlString &sz );

	void AppendTail( const char *psz );
	void AppendTail( char ch );
	void AppendHead( const char *psz );
	void AppendHead( char ch );
	void AppendAt( size_t nPosition, const char *psz );

	void RemoveTail( size_t nCount );
	void RemoveHead( size_t nCount );
	void RemoveAt( size_t nPosition, size_t nCount );

	CUtlString GetExtension( void );

	char *GetString( void );
	size_t GetLenght( void );
	operator char*( void );
	operator CUtlVector<char>&( void );
	CUtlString& operator=(const CUtlString &sz);
	bool operator==(const char* psz);
	bool operator!=(const char* psz);
	bool operator==(CUtlString& string);
	bool operator!=(CUtlString& string);
private:
	CUtlVector<char> m_data = 0;
};

#endif
