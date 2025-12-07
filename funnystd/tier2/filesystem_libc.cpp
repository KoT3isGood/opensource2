#include "tier2/ifilesystem.h"
#include "tier1/interface.h"
#include "tier0/lib.h"

class CLIBCFileHandle : public IFileHandle
{
public:
	FILE *m_pFile;
	size_t m_nSize;

};

class CLIBCFileSystem : public IFileSystem
{
public:
	virtual void Init() override {
	
	};
	virtual void Shutdown() override {};

	virtual IFileHandle *Open( const char *szFileName, int eOpCode ) override 
	{
		const char *szOperation;
		FILE *pFile;
		CLIBCFileHandle *pHandle = NULL;

		switch (eOpCode)
		{
		case FILEMODE_READ:
			szOperation = "rb";
			break;
		default:
			V_printf("Operation is not supported\n");
			break;
		}

		pFile = V_fopen(szFileName, szOperation);
		if (!pFile)
			return NULL;

		pHandle = new CLIBCFileHandle;
		pHandle->m_pFileSystem = this;
		pHandle->m_pFile = pFile;
		pHandle->Seek(SEEKMODE_RELATIVE_END, 0);
		pHandle->m_nSize = pHandle->Tell();
		pHandle->Seek(SEEKMODE_RELATIVE_START, 0);
		return pHandle;
	}
	virtual size_t Write( IFileHandle *pFile, const void *pData, size_t nDataSize ) override
	{
		return 0;
	}
	virtual size_t Read( IFileHandle *pFile, void *pData, size_t nDataSize ) override
	{
		CLIBCFileHandle *pHandle = (CLIBCFileHandle*)pFile;
		if (!pHandle)
			return 0;
		return V_fread( pData, 1, nDataSize, pHandle->m_pFile );
	}

	virtual size_t Seek( IFileHandle *pFile, ESeekMode eSeekMode, size_t nOffset ) override
	{
		CLIBCFileHandle *pHandle = (CLIBCFileHandle*)pFile;
		int eLibcSeekMode = 0;
		
		if (!pHandle)
			return 0;

		switch (eSeekMode) {
		case SEEKMODE_RELATIVE_CURRENT:
			eLibcSeekMode = SEEK_CUR;
			break;
		case SEEKMODE_RELATIVE_END:
			eLibcSeekMode = SEEK_END;
			break;
		case SEEKMODE_RELATIVE_START:
			eLibcSeekMode = SEEK_SET;
			break;
		}

		return V_fseek( pHandle->m_pFile, nOffset, eLibcSeekMode );
	}

	virtual size_t Tell( IFileHandle *pFile ) override
	{
		CLIBCFileHandle *pHandle = (CLIBCFileHandle*)pFile;
		if (!pHandle)
			return 0;
		return V_ftell(pHandle->m_pFile);
	}
	
	virtual size_t Size( IFileHandle *pFile ) override
	{
		CLIBCFileHandle *pHandle = (CLIBCFileHandle*)pFile;
		if (!pHandle)
			return 0;
		return pHandle->m_nSize;
	}



	virtual void Close( IFileHandle *pFile ) override
	{
		CLIBCFileHandle *pHandle = (CLIBCFileHandle*)pFile;
		if (!pHandle)
			return;
		V_fclose(pHandle->m_pFile);
		delete pHandle;
	}
};

EXPOSE_FILESYSTEM(CLIBCFileSystem, "sysfs");
