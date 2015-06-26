// Lock_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <process.h>
#include <time.h> 
#include <typeinfo.h>
#include "Lock.h"
_CCriticalSection g_cs;
_CSemaphore g_Semaphore(1,1);
_CMutex g_mutex;
_CEvent g_event;

void LockerTest(ILock& auto_locker)
{
	clock_t tStart, tEnd;
	int i = 0;
	tStart = clock();
	while (i < 100000)
	{
		CAuto_locker lock(auto_locker);
		i++;
	}
	tEnd = clock();
	std::cout << typeid(auto_locker).name() << " 总共用时: " << (tEnd - tStart) << " ms." << std::endl;
}

unsigned int WINAPI TestThread(void *pParam)
{
	int index = (int)pParam;
	switch (index)
	{
	case 0:
		LockerTest(g_cs);
		break;
	case 1:
		LockerTest(g_mutex);
		break;
	case 2:
		LockerTest(g_Semaphore);
		break;
	case 3:
		LockerTest(g_event);
		break;
	default:
		break;
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hThread[4];
	for (int i = 0; i < 4; i++)
	{
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, &TestThread, (void *)i, 0, 0);
		WaitForSingleObject(hThread[i], INFINITE);
	}	
	WaitForMultipleObjects(4, hThread, TRUE, INFINITE);
	for (int i = 0; i < 4; i++)
	{
		::CloseHandle(hThread[i]);
	}
	system("pause");
	return 0;
}