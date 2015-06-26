#pragma once
#include <windows.h>

class ILock
{
public:
	virtual void lock() = 0;
	virtual void unlock() = 0;
};

//临界区封装
class _CCriticalSection : public ILock
{
public:
	_CCriticalSection(DWORD dwSpinCount = 0);
	~_CCriticalSection();
	DWORD setCSspincount(DWORD dwSpinCount = 0);

	virtual void lock();
	virtual void unlock();
private:
	CRITICAL_SECTION m_cs;
	_CCriticalSection(const _CCriticalSection&);
	_CCriticalSection& operator=(const _CCriticalSection&);
};

//信号量封装
class _CSemaphore : public ILock
{
public:
	_CSemaphore(__in LONG lInitialCount, __in LONG lMaximumCount);
	~_CSemaphore();

	virtual void lock();
	virtual void unlock();
private:
	HANDLE m_hSemaphore;
	_CSemaphore(const _CSemaphore&);
	_CSemaphore& operator=(const _CSemaphore&);
};

//互斥器封装
class _CMutex : public ILock
{
public:
	_CMutex();
	~_CMutex();

	virtual void lock();
	virtual void unlock();
private:
	HANDLE m_hMutex;
	_CMutex(const _CMutex&);
	_CMutex& operator=(const _CMutex&);
};

//事件封装
class _CEvent : public ILock
{
public:
	_CEvent();
	~_CEvent();

	virtual void lock();
	virtual void unlock();
private:
	HANDLE m_hEvent;
	_CEvent(const _CEvent&);
	_CEvent& operator=(const _CEvent&);
};


class CAuto_locker
{
public:
	CAuto_locker(ILock&);
	~CAuto_locker();
private:
	ILock& m_lock;
};
