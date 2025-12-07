//================= Copyright kotofyt, All rights reserved ==================//
// Purpose: Memory buffers for C++. They store data on heap as you have C
// arrays for stack.
//===========================================================================//

#ifndef TIER1_UTL_BUFFER_H
#define TIER1_UTL_BUFFER_H


#include "tier0/mem.h"
#include "tier0/platform.h"
#include "tier0/lib.h"

template <typename T>
class CUtlBuffer;
template <typename T>
class CUtlResizableBuffer;

//-----------------------------------------------------------------------------
// This buffer contains static data allocated on heap.
//-----------------------------------------------------------------------------
template <typename T>
class CUtlBuffer
{
public:
	CUtlBuffer( void );
	CUtlBuffer( size_t nSize );
	CUtlBuffer( const CUtlBuffer<T>& buffer );
	CUtlBuffer( const CUtlResizableBuffer<T>& buffer );
	~CUtlBuffer( void );

	size_t GetSize( void ) const;
	T* GetMemory(void) const;

	operator T*( void ) const;
	T& operator []( const size_t nIndex );
	T operator []( const size_t nIndex ) const;
	CUtlBuffer<T>& operator=(const CUtlBuffer<T>& other);
private:
	T* m_pData = NULL;
	size_t m_nSize = 0;
};

//-----------------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------------
template <typename T>
CUtlBuffer<T>::CUtlBuffer() : m_nSize(0)
{
	m_pData = 0;
}

//-----------------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------------
template <typename T>
CUtlBuffer<T>::CUtlBuffer( size_t nSize ) : m_nSize(nSize)
{
	if ( nSize == 0 )
		nSize = 1;
	m_pData = (T*)V_malloc(sizeof(T)*nSize);
	V_memset(m_pData, 0, sizeof(T)*nSize);
}

//-----------------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------------
template <typename T>
CUtlBuffer<T>::CUtlBuffer( const CUtlBuffer<T>& buffer ) : m_nSize(buffer.m_nSize)
{
	m_pData = (T*)V_malloc(sizeof(T)*buffer.m_nSize);
	V_memcpy(m_pData,buffer.m_pData,sizeof(T)*buffer.m_nSize);
}

//-----------------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------------
template <typename T>
CUtlBuffer<T>::CUtlBuffer( const CUtlResizableBuffer<T>& buffer ) : m_nSize(buffer.m_nSize)
{
	m_pData = (T*)V_malloc(sizeof(T)*buffer.nSize);
	V_memcpy(m_pData,buffer.pData,sizeof(T)*buffer.nSize);
}

template <typename T>
CUtlBuffer<T>::~CUtlBuffer()
{
	if ( m_pData != 0)
		V_free(m_pData);
}
//-----------------------------------------------------------------------------
// Gets memory size.
//-----------------------------------------------------------------------------
template <typename T>
size_t CUtlBuffer<T>::GetSize( void ) const
{
	return m_nSize;
}

//-----------------------------------------------------------------------------
// Gets memory address.
//-----------------------------------------------------------------------------
template <typename T>
T* CUtlBuffer<T>::GetMemory( void ) const
{
	return m_pData;
}

//-----------------------------------------------------------------------------
// Gets memory address using casting.
//-----------------------------------------------------------------------------
template <typename T>
CUtlBuffer<T>::operator T*( void ) const
{
	return m_pData;
}

//-----------------------------------------------------------------------------
// Indexes buffer for writing.
//-----------------------------------------------------------------------------
template <typename T>
T& CUtlBuffer<T>::operator []( const size_t nIndex )
{
	if ( m_pData == 0)
		Plat_FatalErrorFunc("Buffer was not initialized\n");

	if ( nIndex >= m_nSize )
		Plat_FatalErrorFunc("Out of bounds indexing: size is %lu and index is %lu\n", m_nSize/sizeof(T), nIndex);

	return m_pData[nIndex];
}

//-----------------------------------------------------------------------------
// Indexes buffer for reading.
//-----------------------------------------------------------------------------
template <typename T>
T CUtlBuffer<T>::operator []( const size_t nIndex ) const
{
	if ( nIndex >= m_nSize )
		Plat_FatalErrorFunc("Out of bounds indexing: size is %lu and index is %lu\n",m_nSize, nIndex);
	return m_pData[nIndex];
}

//-----------------------------------------------------------------------------
// Assigns buffer.
//-----------------------------------------------------------------------------
template <typename T>
CUtlBuffer<T>& CUtlBuffer<T>::operator=(const CUtlBuffer<T>& other)
{
	if ( this != &other )
	{
		if ( m_pData != 0)
			V_free(m_pData);
		m_pData = (T*)V_malloc(sizeof(T)*other.m_nSize);
		m_nSize = other.m_nSize;
		V_memcpy(m_pData, other.m_pData, sizeof(T)*other.m_nSize);
	}
	return *this;
}

//-----------------------------------------------------------------------------
// This buffer contains static data allocated on heap which can be resized.
//-----------------------------------------------------------------------------
template <typename T>
class CUtlResizableBuffer
{
public:
	CUtlResizableBuffer( void );
	CUtlResizableBuffer( size_t nSize );
	CUtlResizableBuffer( const CUtlBuffer<T>& buffer );
	CUtlResizableBuffer( const CUtlResizableBuffer<T>& buffer );
	~CUtlResizableBuffer();

	size_t GetSize() const;
	size_t GetRealSize() const;
	void Resize( size_t nSize, size_t nDataOffset = 0 );
	void* GetMemory(void) const;
	
	operator T*( void ) const;
	T& operator []( const size_t nIndex );
	T operator []( const size_t nIndex ) const;
	CUtlResizableBuffer<T>& operator=(const CUtlResizableBuffer<T>& other);
private:
	size_t CalculateMemorySize(size_t nSize);
	T* m_pData = NULL;
	size_t m_nSize = 0;
	size_t m_nAllocatedSize = 0;
};



//-----------------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------------
template <typename T>
CUtlResizableBuffer<T>::CUtlResizableBuffer()
{
	m_pData = NULL;
	m_nSize = 0;
	m_nAllocatedSize = 0;
}

//-----------------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------------
template <typename T>
CUtlResizableBuffer<T>::CUtlResizableBuffer( size_t nSize )
{
	Resize(nSize);
}

//-----------------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------------
template <typename T>
CUtlResizableBuffer<T>::CUtlResizableBuffer( const CUtlBuffer<T>& buffer ) : m_nSize(buffer.nSize)
{
}

//-----------------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------------
template <typename T>
CUtlResizableBuffer<T>::CUtlResizableBuffer( const CUtlResizableBuffer<T>& buffer ) : m_nSize(buffer.m_nSize)
{
	*this = buffer;
}

template <typename T>
CUtlResizableBuffer<T>::~CUtlResizableBuffer()
{
	for ( size_t i = 0; i < m_nSize; i++)
		m_pData[i].~T();
	if (m_pData)
		V_free(m_pData);
	m_pData = 0;
	m_nSize = 0;
	m_nAllocatedSize = 0;
};

//-----------------------------------------------------------------------------
// Gets memory size.
//-----------------------------------------------------------------------------
template <typename T>
size_t CUtlResizableBuffer<T>::GetSize( void ) const
{
	return m_nSize;
}

//-----------------------------------------------------------------------------
// Gets memory size.
//-----------------------------------------------------------------------------
template <typename T>
size_t CUtlResizableBuffer<T>::GetRealSize( void ) const
{
	return m_nAllocatedSize;
}

//-----------------------------------------------------------------------------
// Resizes memory.
//-----------------------------------------------------------------------------
template <typename T>
void CUtlResizableBuffer<T>::Resize( size_t nSize, size_t nDataOffset )
{
	if (nSize > m_nAllocatedSize)
	{
		size_t nAllocationSize = CalculateMemorySize(nSize);
		T *pData = (T*)V_malloc(nAllocationSize*sizeof(T));
		for (size_t i = 0; i < m_nSize; i++)
		{
			new (&pData[i+nDataOffset]) T(m_pData[i]);
			m_pData[i].~T();
		}
		V_free(m_pData);
		m_pData = pData;
		m_nAllocatedSize = nAllocationSize;		
	}
	for ( size_t i = m_nSize+nDataOffset; i < nSize; ++i )
		new (&m_pData[i]) T();
	m_nSize = nSize;
}

//-----------------------------------------------------------------------------
// Gets memory address.
//-----------------------------------------------------------------------------
template <typename T>
void* CUtlResizableBuffer<T>::GetMemory( void ) const
{
	return m_pData;
}

//-----------------------------------------------------------------------------
// Gets memory address using casting.
//-----------------------------------------------------------------------------
template <typename T>
CUtlResizableBuffer<T>::operator T*( void ) const
{
	return m_pData;
}

//-----------------------------------------------------------------------------
// Indexes buffer for writing.
//-----------------------------------------------------------------------------
template <typename T>
T& CUtlResizableBuffer<T>::operator []( const size_t nIndex )
{
	if ( m_pData == 0)
		Plat_FatalErrorFunc("Buffer was not initialized\n");

	if ( nIndex >= m_nSize )
		Plat_FatalErrorFunc("Out of bounds indexing: size is %lu and index is %lu\n",m_nSize, nIndex);

	return m_pData[nIndex];
}

//-----------------------------------------------------------------------------
// Indexes buffer for reading.
//-----------------------------------------------------------------------------
template <typename T>
T CUtlResizableBuffer<T>::operator []( const size_t nIndex ) const
{
	if ( nIndex >= m_nSize )
		Plat_FatalErrorFunc("Out of bounds indexing: size is %lu and index is %lu\n",m_nSize, nIndex);
	return m_pData[nIndex];
}

//-----------------------------------------------------------------------------
// Assigns buffer.
//-----------------------------------------------------------------------------
template <typename T>
CUtlResizableBuffer<T>& CUtlResizableBuffer<T>::operator=(const CUtlResizableBuffer<T>& other)
{
	if ( this != &other )
	{
		if (m_pData)
		{
			for (size_t i = 0; i < m_nSize; i++)
				m_pData[i].~T();
			V_free(m_pData);
		}
		m_pData = (T*)V_malloc(other.m_nAllocatedSize*sizeof(T));
		m_nAllocatedSize = other.m_nAllocatedSize;
		m_nSize = other.m_nSize;
		for ( size_t i = 0; i < other.m_nSize; i++)
			new (&m_pData[i]) T(other.m_pData[i]);
	}
	return *this;
}

//-----------------------------------------------------------------------------
// Calculates memory size that is 
//-----------------------------------------------------------------------------
template <typename T>
size_t CUtlResizableBuffer<T>::CalculateMemorySize(size_t nSize)
{
	size_t x = nSize;
	if (x == 0) return 1;
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
#if SIZE_MAX > UINT32_MAX
	x |= x >> 32;
#endif
	return x + 1;
}

#endif
