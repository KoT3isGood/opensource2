#include "tier1/utlmutex.h"
#include "pthread.h"

CUtlLock::CUtlLock()
{
	pthread_mutex_init(&m_lock);
};

CUtlLock::~CUtlLock()
{
	
};

void CUtlLock::Lock()
{

};

void CUtlLock::Unlock()
{

};
