/*
 * ClassFactory.cpp
 */
#include "ClassFactory.h"

#include <new>
#include <string>

#include <windows.h>
#include <shlwapi.h>	// Shlwapi.lib

#include "dll.h"



///////////////////////////////////////////////////////////////////////////////
//
//	ClassFacotry
// 
///////////////////////////////////////////////////////////////////////////////
ClassFacotry::ClassFacotry(CreateInstanceFunctionPointer fp)
	: mRefCounter(1)
	, mFuncPointer(fp)
{
	DllAddRef();
}

ClassFacotry::~ClassFacotry()
{
	DllRelease();
}


//-----------------------------------------------------------------------------
// ClassFacotry : IClassFactory functions
//-----------------------------------------------------------------------------
HRESULT ClassFacotry::QueryInterface(REFIID riid, void **ppvObject)
{
	static const QITAB qit[] = {
		QITABENT(ClassFacotry, IClassFactory),
		{ 0 }
	};
	return QISearch(this, qit, riid, ppvObject);
}

ULONG ClassFacotry::AddRef(void)
{
	return InterlockedIncrement(&this->mRefCounter);
}

ULONG ClassFacotry::Release(void)
{
	unsigned long ref = InterlockedDecrement(&this->mRefCounter);
	if (ref == 0) {
		delete this;
	}
	return ref;
}

HRESULT ClassFacotry::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
{
	if (pUnkOuter) return CLASS_E_NOAGGREGATION;

	return this->mFuncPointer(riid, ppvObject);
}

HRESULT ClassFacotry::LockServer(BOOL fLock)
{
	if (fLock) {
		DllAddRef();
	} else {
		DllRelease();
	}
	return S_OK;
}

