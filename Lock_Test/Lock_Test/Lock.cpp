#include "stdafx.h"
#include "Lock.h"
#include <assert.h>

_CCriticalSection::_CCriticalSection(DWORD dwSpinCount /* = 0 */)
{
	if (dwSpinCount > 0)
	{
		::InitializeCriticalSectionAndSpinCount(&m_cs, dwSpinCount);
	}
	else
	{
		::InitializeCriticalSection(&m_cs);
	}
}

_CCriticalSection::~_CCriticalSection()
{
	::DeleteCriticalSection(&m_cs);
}

void _CCriticalSection::lock()
{
	::EnterCriticalSection(&m_cs);
}

void _CCriticalSection::unlock()
{
	::LeaveCriticalSection(&m_cs);
}

DWORD _CCriticalSection::setCSspincount(DWORD dwSpinCount /* = 0 */)
{
	return ::SetCriticalSectionSpinCount(&m_cs, dwSpinCount);
}


_CSemaphore::_CSemaphore(__in LONG lInitialCount, __in LONG lMaximumCount)
{
	assert(lInitialCount >= 0 && lMaximumCount >= 0 && lInitialCount <= lMaximumCount);
	m_hSemaphore = ::CreateSemaphore(NULL, lInitialCount, lMaximumCount, NULL);

	//Ê§°Ü·µ»ØNULL
	assert(m_hSemaphore);
}

_CSemaphore::~_CSemaphore()
{
	CloseHandle(m_hSemaphore);
}

void _CSemaphore::lock()
{
	WaitForSingleObject(m_hSemaphore, INFINITE);
}

void _CSemaphore::unlock()
{
	::ReleaseSemaphore(m_hSemaphore, 1, NULL);
}


_CMutex::_CMutex()
{
	m_hMutex = ::CreateMutex(NULL, FALSE, NULL);
	assert(m_hMutex);
}

_CMutex::~_CMutex()
{
	::CloseHandle(m_hMutex);
}

void _CMutex::lock()
{
	WaitForSingleObject(m_hMutex, INFINITE);
}

void _CMutex::unlock()
{
	::ReleaseMutex(m_hMutex);
}


_CEvent::_CEvent()
{
	m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);
	assert(m_hEvent);
}

_CEvent::~_CEvent()
{
	::CloseHandle(m_hEvent);
}

void _CEvent::lock()
{
	WaitForSingleObject(m_hEvent, INFINITE);
}

void _CEvent::unlock()
{
	SetEvent(m_hEvent);
}



CAuto_locker::CAuto_locker(ILock& locker) : m_lock(locker)
{
	m_lock.lock();
}

CAuto_locker::~CAuto_locker()
{
	m_lock.unlock();
}