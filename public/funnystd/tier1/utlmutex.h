//================= Copyright kotofyt, All rights reserved ==================//
// Purpose: Mutexes for C++
//===========================================================================//

#ifndef TIER1_UTL_MUTEX_H
#define TIER1_UTL_MUTEX_H

#include "pthread.h"

class CUtlLock
{
public:
	CUtlLock();
	~CUtlLock();

	CUtlLock(const CUtlLock&) = delete;
	CUtlLock& operator=(const CUtlLock&) = delete;

	void Lock();
	void Unlock();
private:
	pthread_mutex_t m_lock;
};

#endif
