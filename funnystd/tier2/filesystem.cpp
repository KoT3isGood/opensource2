#include "tier2/ifilesystem.h"
#include "tier1/interface.h"
#include "tier0/lib.h"

size_t IFileHandle::Write( const void *pData, size_t nDataSize )
{
	return m_pFileSystem->Write(this, pData, nDataSize);
}

size_t IFileHandle::Read( void *pData, size_t nDataSize )
{
	return m_pFileSystem->Read(this, pData, nDataSize);
}

size_t IFileHandle::Seek( ESeekMode eSeekMode, size_t nOffset )
{
	return m_pFileSystem->Seek(this, eSeekMode, nOffset);	
}

size_t IFileHandle::Tell( void )
{
	return m_pFileSystem->Tell(this);
}

size_t IFileHandle::Size( void )
{
	return m_pFileSystem->Size(this);
}

void IFileHandle::Close( void )
{
	m_pFileSystem->Close(this);
}


class CFileSystem : public IFileSystem
{
public:
	virtual void Init() override {
	};
	virtual void Shutdown() override {

	};

	virtual IFileHandle *Open( const char *szFileName, int eOpCode ) override 
	{
		IFileSystem *pFileSystem;
		IFileHandle *pHandle = NULL;

		for ( pFileSystem = g_pFileSystems; pFileSystem; pFileSystem = pFileSystem->m_pNext )
		{
			pHandle = pFileSystem->Open(szFileName, eOpCode);
			if (pHandle) return pHandle;
		}

		return NULL;
	}

	virtual size_t Write( IFileHandle *pFile, const void *pData, size_t nDataSize ) override
	{
		return pFile->Write(pData, nDataSize);
	}

	virtual size_t Read( IFileHandle *pFile, void *pData, size_t nDataSize ) override
	{
		return pFile->Read(pData, nDataSize);
	}	

	virtual size_t Seek( IFileHandle *pFile, ESeekMode eSeekMode, size_t nOffset ) override
	{
		return pFile->Seek(eSeekMode, nOffset);
	}

	virtual size_t Tell( IFileHandle *pFile ) override
	{
		return pFile->Tell();
	}
	virtual size_t Size( IFileHandle *pFile ) override
	{
		return pFile->Size();
	}
	virtual void Close( IFileHandle *pFile ) override
	{

	}
};


static CFileSystem s_fileSystem;
IFileSystem *filesystem = &s_fileSystem;

IFileSystem *g_pFileSystems;

CFileSystemRegistry::CFileSystemRegistry( InstantiateFileSystemFn fn, const char *szFileSystem )
{
	IFileSystem *pFileSystem = fn();
	pFileSystem->RegisterFileSystem();
}

void IFileSystem::RegisterFileSystem()
{
	m_pNext = g_pFileSystems;
	g_pFileSystems = this;
};
