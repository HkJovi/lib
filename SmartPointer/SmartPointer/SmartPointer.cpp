// SmartPointer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SmartPtr.hpp"
#include <iostream>
#include <process.h>
#include <memory>

#define THREADCOUNT 60

typedef struct _PERSON_
{
	char szName[20];
	int nAge;
	~_PERSON_()
	{
		std::cout << "Person Distructor!"<< std::endl;
	}
}PERSON;

SmartPtr<PERSON> g_p(new PERSON{ "g_test", 12 });

unsigned int __stdcall testThread(void *pParam)
{
	SmartPtr<PERSON> sp((SmartPtr<PERSON> *)pParam);
	g_p = sp;
	std::cout << sp->nAge << std::endl;
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	do 
	{
		HANDLE hThread[THREADCOUNT];
		SmartPtr<PERSON> p0(new PERSON{ "Jovi", 12 });
		SmartPtr<PERSON> p1(new PERSON{ "Solanin", 13 });

		SmartPtr<PERSON> p2(p1);
		const SmartPtr<PERSON> p3(p1);

		SmartPtr<PERSON> p4(p3);
		std::cout << (p3 == p1) << std::endl;
		std::cout << (p2 == p0) << std::endl;
		std::cout << (p3 != p1) << std::endl;
		std::cout << (p2 != p0) << std::endl;
		p4 = p0;
		SmartPtr<PERSON> *p = new SmartPtr<PERSON>(p1);
		for (int i = 0; i < THREADCOUNT; i++)
		{
			hThread[i] = (HANDLE)_beginthreadex(NULL, 0, testThread, (void *)p, 0, 0);
//			WaitForSingleObject(hThread[i], INFINITE);
		}
		WaitForMultipleObjects(3, hThread, TRUE, INFINITE);
		delete p;
	} while (0);

	system("pause");
	return 0;
}

