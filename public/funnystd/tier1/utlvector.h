//================= Copyright kotofyt, All rights reserved ==================//
// Purpose: Own implementation of vectors, it is better to use them in local
// namespaces for own .
//===========================================================================//

#ifndef TIER1_UTL_VECTOR_H
#define TIER1_UTL_VECTOR_H

#include "tier0/lib.h"
#include "tier1/utlbuffer.h"
#include "tier1/utlinitlist.h"

//-----------------------------------------------------------------------------
// Basic vector implementation. There isn't much in them but they work.
//-----------------------------------------------------------------------------
template<typename T>
class CUtlVector
{
public:
	CUtlVector( void );
	CUtlVector( size_t nSize );
	CUtlVector( CUtlInitializerList<T> initalizerList );
	CUtlVector( const CUtlVector &vector );
	~CUtlVector();

	void AppendHead( const T &data );
	void AppendHead( const T *pData, size_t n );
	void AppendTail( const T &data );
	void AppendTail( const T *data, size_t n );
	void AppendAt( size_t nIndex, const T &data );
	void AppendAt( size_t nIndex, const T *data, size_t n );

	void RemoveHead();
	void RemoveHead( size_t n );
	void RemoveTail();
	void RemoveTail( size_t n );
	void RemoveAt( size_t nIndex );
	void RemoveAt( size_t nIndex, size_t n );

	T *GetData( void );
	size_t GetSize( void );
	void Resize( size_t nSize, size_t nDataOffset = 0 );
	void Reserve( size_t nSize, size_t nDataOffset = 0 );

	T &operator[]( size_t nIndex );
	T &operator[]( size_t nIndex ) const;
	CUtlVector<T> &operator=(const CUtlVector<T> &vec);

	// Iterator stuff
	// Do we really need it?
	struct Iterator {
		T *m_pCurrent;
		Iterator( T *pCurrent ) : m_pCurrent(pCurrent) {}
		T& operator*( void ) const { return *m_pCurrent;}
		Iterator& operator++( void ) {
			++m_pCurrent;
			return *this;
		}
		bool operator!=( const Iterator& other ) const
		{
			return m_pCurrent != other.m_pCurrent;
		}
	};


	Iterator begin( void ) const
	{
		return Iterator((T*)m_data.GetMemory());
	}
	Iterator end( void ) const
	{
		return Iterator((T*)m_data.GetMemory()+m_nSize);
	}

private:
	size_t m_nSize = 0;
	CUtlResizableBuffer<T> m_data;
};

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
template<typename T>
CUtlVector<T>::CUtlVector( void )
{

};

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
template<typename T>
CUtlVector<T>::CUtlVector( size_t nSize )
{
	m_data.Resize(nSize);
	V_memset(m_data.GetMemory(),0,m_data.GetSize()*sizeof(T));
	m_nSize = nSize;
};

//----------------------------------------------------------------------------
// Fancy constructor
//----------------------------------------------------------------------------
template<typename T>
CUtlVector<T>::CUtlVector( CUtlInitializerList<T> initalizerList )
{
	m_data.Resize(initalizerList.size());
	V_memset(m_data.GetMemory(),0,m_data.GetSize()*sizeof(T));
	m_nSize = m_data.GetSize();
	for (size_t i = 0; i<m_nSize; i++)
		m_data[i] = initalizerList.begin()[i];
}

template<typename T>
CUtlVector<T>::CUtlVector( const CUtlVector& vector )
{
	*this = vector;
}
template<typename T>
CUtlVector<T>::~CUtlVector()
{
}

template<typename T>
void CUtlVector<T>::AppendHead( const T &data )
{
	AppendHead(&data,1);
}

template<typename T>
void CUtlVector<T>::AppendHead( const T *pData, size_t n )
{
	size_t nOldSize = m_nSize;
	m_data.Resize(m_nSize+n);
	for (size_t i = nOldSize; i > 0; --i)
	{
		new (&m_data[i + n - 1]) T(m_data[i - 1]);
		m_data[i - 1].~T();
	}
	for (size_t i = 0; i < n; ++i)
	{
		new (&m_data[i]) T(pData[i]);
	}
	m_nSize+=n;
}

template<typename T>
void CUtlVector<T>::AppendTail( const T &data )
{
	AppendTail(&data,1);
}

template<typename T>
void CUtlVector<T>::AppendTail( const T *pData, size_t n )
{
	m_data.Resize(m_data.GetSize()+n);
	for ( size_t i = 0; i < n; i++ )
		new (&m_data[i+m_nSize]) T(pData[i]);
	m_nSize+=n;
}

template<typename T>
void CUtlVector<T>::AppendAt( size_t nIndex, const T &data )
{
	AppendAt(nIndex, &data, 1);
}

template<typename T>
void CUtlVector<T>::AppendAt( size_t nIndex, const T *pData, size_t n )
{
	size_t nOldSize = m_nSize;
	m_data.Resize(m_nSize+n);
	for (size_t i = nOldSize; i > nIndex; --i)
	{
		new (&m_data[i + n - 1]) T(m_data[i - 1]);
		m_data[i - 1].~T();
	}
	for (size_t i = 0; i < n; ++i)
	{
		new (&m_data[nIndex + i]) T(pData[i]);
	}
	m_nSize+=n;
}

template<typename T>
void CUtlVector<T>::RemoveHead()
{
	RemoveHead( 1 );
}
template<typename T>
void CUtlVector<T>::RemoveHead( size_t n )
{
	RemoveAt(0, n);
}
template<typename T>
void CUtlVector<T>::RemoveTail()
{
	RemoveTail( 1 );
}
template<typename T>
void CUtlVector<T>::RemoveTail( size_t n )
{
	if (n > m_nSize)
		n = m_nSize;
	m_data.Resize(m_nSize-n);
	m_nSize -= n;
}
template<typename T>
void CUtlVector<T>::RemoveAt( size_t nIndex )
{
	RemoveAt( nIndex, 1 );
}
template<typename T>
void CUtlVector<T>::RemoveAt( size_t nIndex, size_t n )
{
	if ( nIndex >= m_nSize )
	{
		return;
	}
	if ( nIndex + n > m_nSize )
	{
		n = m_nSize - nIndex;
	}
	for ( size_t i = nIndex; i < nIndex + n; i++ )
	{
		m_data[i].~T();
	}

	size_t nElementsToMove = m_nSize - (nIndex + n);
	for ( size_t i = 0; i < nElementsToMove; i++ )
	{
		new (&m_data[nIndex + i]) T(m_data[nIndex + n + i]);
		m_data[nIndex + n + i].~T();
	}
	m_nSize -= n;
}

template<typename T>
T *CUtlVector<T>::GetData( void )
{
	return (T*)m_data.GetMemory();
}

template<typename T>
size_t CUtlVector<T>::GetSize( void )
{
	return m_nSize;
}

template<typename T>
void CUtlVector<T>::Resize( size_t nSize, size_t nDataOffset )
{
	m_data.Resize(nSize, nDataOffset);
	m_nSize = nSize;
}
template<typename T>
void CUtlVector<T>::Reserve( size_t nSize, size_t nDataOffset )
{
	m_data.Resize(nSize, nDataOffset);
}
template<typename T>
CUtlVector<T> &CUtlVector<T>::operator=(const CUtlVector<T> &vec)
{
	if (this != &vec)
	{
		m_nSize = vec.m_nSize;
		m_data = vec.m_data;
	}
	return *this;
}

template<typename T>
T &CUtlVector<T>::operator[]( size_t nIndex )
{
	return m_data[nIndex];
}

template<typename T>
T &CUtlVector<T>::operator[]( size_t nIndex ) const
{
	return m_data[nIndex];
}


//-----------------------------------------------------------------------------
// Self referencing arrays are quite cool.
// They allow to append stuff in head and tail of the array and use less memory 
// copying. Downside is their indexing is O(n/2)
//
// Implements the same stuff as CUtlVector does.
//-----------------------------------------------------------------------------
template<typename T>
class CUtlSelfReferencingVector
{
private:
	template<typename A>
	struct SelfData_t;
public:
	CUtlSelfReferencingVector();
	~CUtlSelfReferencingVector();

	void AppendHead( const T& data );
	void AppendTail( const T& data );
	void AppendAt( size_t nIndex, const T& data );

	void RemoveHead( void );
	void RemoveTail( void );
	void RemoveAt( size_t nIndex );

	size_t GetSize();

	T operator[]( size_t nIndex );
	T& operator[]( size_t nIndex ) const;
	

	// Iterator stuff
	struct Iterator {
		SelfData_t<T> *m_pCurrent;
		Iterator( SelfData_t<T> *pCurrent ) : m_pCurrent(pCurrent) {}
		T& operator*( void ) const { return m_pCurrent->data;}
		Iterator& operator++( void ) {
			if (m_pCurrent) m_pCurrent = m_pCurrent->pNext;
			return *this;
		}
		bool operator!=( const Iterator& other ) const
		{
			return m_pCurrent != other.m_pCurrent;
		}
	};

	Iterator begin( void ) const
	{
		return Iterator(m_pHead);
	}
	Iterator end( void ) const
	{
		return Iterator(NULL);
	}

private:
	size_t m_nSize = 0;

	template<typename A>
	struct SelfData_t
	{
		SelfData_t *pNext = NULL;
		SelfData_t *pPrev = NULL;
		A data;
	};
	SelfData_t<T> *m_pTail = NULL;
	SelfData_t<T> *m_pHead = NULL;
};


//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
template<typename T>
CUtlSelfReferencingVector<T>::CUtlSelfReferencingVector()
{

};

//----------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------
template<typename T>
CUtlSelfReferencingVector<T>::~CUtlSelfReferencingVector()
{
	SelfData_t<T> *pNext = NULL;
	for (SelfData_t<T> *pCurrent = m_pHead; pCurrent; pCurrent=pNext)
	{
		pNext = pCurrent->pNext;
		delete pCurrent;
	}
};

//----------------------------------------------------------------------------
// Inserts new element in the start of the vector. 
//----------------------------------------------------------------------------
template<typename T>
void CUtlSelfReferencingVector<T>::AppendHead( const T& data )
{
	SelfData_t<T>* pData = new SelfData_t<T>;
	pData->data = data;
	pData->pNext = m_pHead;
	if (m_pHead)
		m_pHead->pPrev = pData;
	pData->pPrev = 0;
	m_pHead = pData;
	if (m_pTail == 0)
		m_pTail = m_pHead;
	m_nSize++;
};

//----------------------------------------------------------------------------
// Inserts new element in the end of the vector. 
//----------------------------------------------------------------------------
template<typename T>
void CUtlSelfReferencingVector<T>::AppendTail( const T& data )
{
	SelfData_t<T>* pData = new SelfData_t<T>;
	pData->data = data;
	if (m_pTail)
		m_pTail->pNext = pData;
	pData->pPrev = m_pTail;
	m_pTail = pData;
	if (m_pHead == 0)
		m_pHead = m_pTail;
	
	m_nSize++;
};

//----------------------------------------------------------------------------
// Removes element in the start of the vector. 
//----------------------------------------------------------------------------
template<typename T>
void CUtlSelfReferencingVector<T>::RemoveHead( void )
{
	if (m_pHead == m_pTail)
		m_pHead = 0;
};
//----------------------------------------------------------------------------
// Removes element in the end of the vector. 
//----------------------------------------------------------------------------
template<typename T>
void CUtlSelfReferencingVector<T>::RemoveTail( void )
{

}

#endif
