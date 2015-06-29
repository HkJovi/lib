#pragma once
#include "stdafx.h"
#include <assert.h>
#include <windows.h>

//#define DEBUG_SMARTPTR

template<typename T>
class SmartPtr;

template <typename T>
class RefPtr
{
	friend class SmartPtr<T>;
	explicit RefPtr(T *p) :pointer(p), nUse(0)
	{	
		assert(pointer);
#ifdef DEBUG_SMARTPTR
		std::cout << "Create Pointer!" << std::endl;
#endif
	}

	RefPtr(const RefPtr&)
	{

	}

	RefPtr& operator= (const RefPtr & ref)
	{

	}

	~RefPtr()
	{
#ifdef DEBUG_SMARTPTR
		std::cout << "Delete Pointer!" << std::endl;
#endif
		assert(pointer);
		if (pointer != NULL)
		{
			delete pointer;
			pointer = NULL;
		}	
	}

	unsigned int AddRefCount()
	{
		return InterlockedIncrement((unsigned int*)&nUse);
	}

	unsigned int SubRefCount()
	{
		return InterlockedDecrement((unsigned int*)&nUse);
	}

	bool AddRefCount_lock()
	{
		for (;;)
		{
			unsigned int temp = nUse;
			if (temp == 0)
			{
				return false;
			}
			if (InterlockedCompareExchange((unsigned int *)&nUse, temp + 1, temp) == temp)
			{
				return true;
			}
		}		
	}

	volatile unsigned int nUse;
	T *pointer;
};

template<typename T>
class SmartPtr
{
public:
	explicit SmartPtr(T *pointer) :ptr(new RefPtr<T>(pointer))
	{
		assert(pointer);
#ifdef DEBUG_SMARTPTR
		std::cout << "Create SmartPointer!" << std::endl;
#endif
		ptr->AddRefCount();
	}

	explicit SmartPtr(const SmartPtr<T>& sp) :ptr(sp.ptr)
	{
#ifdef DEBUG_SMARTPTR
		std::cout << "Copy0 SmartPointer!" << std::endl;
#endif
		ptr->AddRefCount();
	}

	explicit SmartPtr(const SmartPtr<T>* sp) :ptr(sp->ptr)
	{
#ifdef DEBUG_SMARTPTR
		std::cout << "Copy1 SmartPointer!" << std::endl;
#endif
		ptr->AddRefCount();
	}

	SmartPtr& operator=(const SmartPtr<T>& sp)
	{
		if (sp.ptr != ptr)
		{
			if (sp.ptr->AddRefCount_lock())
			{
				if (ptr->SubRefCount() == 0)
				{
					delete ptr;
				}
				ptr = sp.ptr;
			}
		}
#ifdef DEBUG_SMARTPTR
		std::cout << "Copy2 SmartPointer!" << std::endl;
#endif
		return *this;
	}

	T* operator->()
	{
		return GetPtr();
	}

	T* operator->() const
	{
		return GetPtr();
	}

	T& operator*()
	{
		return *ptr->pointer;
	}

	T& operator*() const
	{
		return *ptr->pointer;
	}

	bool operator!()
	{
		return !ptr;
	}

	~SmartPtr()
	{
		if (ptr->SubRefCount() == 0)
		{
			delete ptr;
		}
#ifdef DEBUG_SMARTPTR
		std::cout << "Delete SmartPointer!" << std::endl;
#endif
	}

	int GetRefCount() const
	{
		return ptr->nUse;
	}

	bool isNull()
	{
		return ptr->pointer == NULL;
	}

	T* GetPtr() const
	{
		assert(ptr->pointer);
		return ptr->pointer;
	}

	//返回对象
	T GetValue() const
	{
		assert(ptr->pointer);
		return *ptr->pointer;
	}

private:
	RefPtr<T> *ptr;
};

//兼容const比较
template<typename T>
inline bool operator==(const SmartPtr<T>& a,const SmartPtr<T>& b)
{
	return a.GetPtr() == b.GetPtr();
}
template<typename T>
inline bool operator!=(const SmartPtr<T>& a,const SmartPtr<T>& b)
{
	return a.GetPtr() != b.GetPtr();
}