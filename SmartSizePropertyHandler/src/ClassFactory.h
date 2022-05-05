/*
 * ClassFactory.h
 * 
 * the class factory for the SmartSizePropertyHandler class.
 */
#ifndef __ClassFactory_H__
#define __ClassFactory_H__

#include <windows.h>


typedef HRESULT (*CreateInstanceFunctionPointer)(REFIID riid, void **ppvObject);

class ClassFacotry : public IClassFactory
{
public:
	ClassFacotry(CreateInstanceFunctionPointer fp);

	// IClassFactory
	HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject) override;
	ULONG __stdcall AddRef(void) override;
	ULONG __stdcall Release(void) override;
	HRESULT __stdcall CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject) override;
	HRESULT __stdcall LockServer(BOOL fLock) override;

private:
	~ClassFacotry();

	long mRefCounter;
	CreateInstanceFunctionPointer mFuncPointer;
};


#endif /* __ClassFactory_H__ */
