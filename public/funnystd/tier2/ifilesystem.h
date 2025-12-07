#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "iappsystem.h"
#include "tier0/platform.h"

enum EFileMode
{
	FILEMODE_READ = 0x01,
	FILEMODE_WRITE = 0x02,
	FILEMODE_APPEND = 0x04,
};

enum ESeekMode
{
	SEEKMODE_RELATIVE_CURRENT,
	SEEKMODE_RELATIVE_START,
	SEEKMODE_RELATIVE_END,
};

abstract_class IFileSystem;

class IFileHandle
{
public:
	IFileSystem *m_pFileSystem;

	size_t Write( const void *pData, size_t nDataSize );
	size_t Read( void *pData, size_t nDataSize );
	size_t Seek( ESeekMode eSeekMode, size_t nOffset );
	size_t Tell( void );
	size_t Size( void );
	void Close( void );


};


//----------------------------------------------------------------------------
// IFileSystem is an app system which manages files, could have different 
// file systems etc. Because of that there is base file system which manages 
// others. PAK files are opened first, then mounted stuff comes second and 
// host's system last.
//----------------------------------------------------------------------------
abstract_class IFileSystem: public IAppSystem
{
public:
	virtual IFileHandle *Open( const char *szFileName, int eOpCode ) = 0;	
	virtual size_t Write( IFileHandle *pFile, const void *pData, size_t nDataSize ) = 0;
	virtual size_t Read( IFileHandle *pFile, void *pData, size_t nDataSize ) = 0;

	virtual size_t Seek( IFileHandle *pFile, ESeekMode eSeekMode, size_t nOffset ) = 0;
	virtual size_t Tell( IFileHandle *pFile ) = 0;
	virtual size_t Size( IFileHandle *pFile ) = 0;

	virtual void Close( IFileHandle *pFile ) = 0;

	IFileSystem *m_pNext;
	
	void RegisterFileSystem();
};

extern IFileSystem *filesystem;
extern IFileSystem *g_pFileSystems;

typedef IFileSystem *( *InstantiateFileSystemFn )( void );
class CFileSystemRegistry
{
public:
	CFileSystemRegistry( InstantiateFileSystemFn fn, const char *szFileSystem );
};

#define EXPOSE_FILESYSTEM( className, filesystemName ) \
	static IFileSystem *__Create##className##_filesystem() { return ( IFileSystem* )( new className ); }; \
	static CFileSystemRegistry __Create##className##_registry( __Create##className##_filesystem, filesystemName );


#endif
