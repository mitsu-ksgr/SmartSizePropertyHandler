/*
 * SmartSizePropertyHandler.cpp
 */
#include "SmartSizePropertyHandler.h"

#include <new>
#include <string>

#include <windows.h>
#include <msxml6.h>
#include <propsys.h>		// Propsys.lib
#include <propkey.h>
#include <propvarutil.h>
#include <shlwapi.h>		// Shlwapi.lib
#include <winnt.h>

#include "dll.h"

//-----------------------------------------------------------------------------
// Utils
//-----------------------------------------------------------------------------
namespace {
	std::wstring toSmartSize(unsigned long long bytes)
	{
		constexpr const unsigned long long KiB = 1024L;
		constexpr const unsigned long long MiB = KiB * 1024L;
		constexpr const unsigned long long GiB = MiB * 1024L;
		constexpr const unsigned long long TiB = GiB * 1024L;
		constexpr const unsigned long long PiB = TiB * 1024L;

		if (bytes / PiB > 0) {
			return std::to_wstring(bytes / PiB) + L"." +
				   std::to_wstring(bytes % PiB)[0] + L" PiB";
		}
		if (bytes / TiB > 0) {
			return std::to_wstring(bytes / TiB) + L"." +
				   std::to_wstring(bytes % TiB)[0] + L" TiB";
		}
		if (bytes / GiB > 0) {
			return std::to_wstring(bytes / GiB) + L"." +
				   std::to_wstring(bytes % GiB)[0] + L" GiB";
		}
		if (bytes / MiB > 0) {
			return std::to_wstring(bytes / MiB) + L"." +
				   std::to_wstring(bytes % MiB)[0] + L" MiB";
		}
		if (bytes / KiB > 0) {
			return std::to_wstring(bytes / KiB) + L"." +
				   std::to_wstring(bytes % KiB)[0] + L" KiB";
		}
		return std::to_wstring(bytes) + L" B";
	}

	// Ref: SmartSize.propdesc
	const PROPERTYKEY PKEY_SmartSize = {
		SMARTSIZE_PROP_KEY,
		SMARTSIZE_PROP_PROP_ID
	};
}


///////////////////////////////////////////////////////////////////////////////
//
//	SmartSizePropertyHandler
// 
///////////////////////////////////////////////////////////////////////////////
class SmartSizePropertyHandler
	: public IPropertyStore
	, public IPropertyStoreCapabilities
	, public IInitializeWithStream
{
public:
	SmartSizePropertyHandler();
	~SmartSizePropertyHandler();

	// IUnknown
	HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject) override;
	ULONG __stdcall AddRef(void) override;
	ULONG __stdcall Release(void) override;

	// IPropertyStore
	HRESULT __stdcall GetCount(DWORD * cProps) override;
	HRESULT __stdcall GetAt(DWORD iProp, PROPERTYKEY * pkey) override;
	HRESULT __stdcall GetValue(REFPROPERTYKEY key, PROPVARIANT * pv) override;
	HRESULT __stdcall SetValue(REFPROPERTYKEY key, REFPROPVARIANT propvar) override;
	HRESULT __stdcall Commit(void) override;

	// IPropertyStoreCapabilities
	HRESULT __stdcall IsPropertyWritable(REFPROPERTYKEY key) override;

	// IInitializeWithStream
	HRESULT __stdcall Initialize(IStream * pstream, DWORD grfMode) override;


private:
	long _cRef;
	IPropertyStoreCache *_pCache;	// internal value cache to abstract IProperty Store
};


//-----------------------------------------------------------------------------
// SmartSizePropertyHandler
//-----------------------------------------------------------------------------
SmartSizePropertyHandler::SmartSizePropertyHandler()
	: _cRef(1)
	, _pCache(nullptr)
{
	DllAddRef();
}

SmartSizePropertyHandler::~SmartSizePropertyHandler()
{
	if (this->_pCache) {
		this->_pCache->Release();
		this->_pCache = nullptr;
	}
	DllRelease();
}


//-----------------------------------------------------------------------------
// SmartSizePropertyHandler : IUnknown functions.
//-----------------------------------------------------------------------------
HRESULT SmartSizePropertyHandler::QueryInterface(REFIID riid, void **ppvObject)
{
	static const QITAB qit[] = {
		QITABENT(SmartSizePropertyHandler, IPropertyStore),
		QITABENT(SmartSizePropertyHandler, IPropertyStoreCapabilities),
		QITABENT(SmartSizePropertyHandler, IInitializeWithStream),
		{ 0 },
	};
	return QISearch(this, qit, riid, ppvObject);
}

ULONG SmartSizePropertyHandler::AddRef(void)
{
	return InterlockedIncrement(&this->_cRef);
}

ULONG SmartSizePropertyHandler::Release(void)
{
	unsigned long ref = InterlockedDecrement(&this->_cRef);
	if (!ref) {
		delete this;
	}
	return ref;
}


//-----------------------------------------------------------------------------
// SmartSizePropertyHandler : IPropertyStore functions.
//-----------------------------------------------------------------------------
HRESULT SmartSizePropertyHandler::GetCount(DWORD *cProps)
{
	*cProps = 0;
	return this->_pCache ? this->_pCache->GetCount(cProps) : E_UNEXPECTED;
}

HRESULT SmartSizePropertyHandler::GetAt(DWORD iProp, PROPERTYKEY *pkey)
{
	*pkey = PKEY_Null;
	return this->_pCache ? this->_pCache->GetAt(iProp, pkey) : E_UNEXPECTED;
}

HRESULT SmartSizePropertyHandler::GetValue(REFPROPERTYKEY key, PROPVARIANT *pv)
{
	PropVariantInit(pv);
	return this->_pCache ? this->_pCache->GetValue(key, pv) : E_UNEXPECTED;
}

HRESULT SmartSizePropertyHandler::SetValue(REFPROPERTYKEY key, REFPROPVARIANT propvar)
{
	return STG_E_ACCESSDENIED;
}

HRESULT SmartSizePropertyHandler::Commit(void)
{
	return STG_E_ACCESSDENIED;
}


//-----------------------------------------------------------------------------
// SmartSizePropertyHandler : IPropertyStoreCapabilities functions.
//-----------------------------------------------------------------------------
HRESULT SmartSizePropertyHandler::IsPropertyWritable(REFPROPERTYKEY key)
{
	return S_FALSE;
}


//-----------------------------------------------------------------------------
// SmartSizePropertyHandler : IInitializeWithStream functions.
//-----------------------------------------------------------------------------
HRESULT SmartSizePropertyHandler::Initialize(IStream *pstream, DWORD grfMode)
{
	HRESULT hr = E_UNEXPECTED;

	// Create cache
	if (!this->_pCache) {
		hr = PSCreateMemoryPropertyStore(IID_PPV_ARGS(&this->_pCache));
		if (FAILED(hr)) return hr;
	}

	// Get file size.
	STATSTG stat = { 0 };
	hr = pstream->Stat(&stat, 0);
	if (FAILED(hr)) return hr;
	std::wstring smart_size = toSmartSize(stat.cbSize.QuadPart);

	// Save to cache
	PROPVARIANT pv = {};
	PropVariantInit(&pv);

	do {
		hr = InitPropVariantFromString(smart_size.c_str(), &pv);
		if (FAILED(hr)) break;

		hr = PSCoerceToCanonicalValue(PKEY_SmartSize, &pv);
		if (FAILED(hr)) break;

		hr = this->_pCache->SetValueAndState(
			PKEY_SmartSize,
			&pv,
			PSC_READONLY
		);
	} while (false);

	PropVariantClear(&pv);

	return hr;
}


//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
HRESULT SmartSizePropertyHandler_CreateInstance(REFIID riid, void ** ppv)
{
	*ppv = nullptr;
	SmartSizePropertyHandler * p_new = new(std::nothrow) SmartSizePropertyHandler;

	HRESULT hr = p_new ? S_OK : E_OUTOFMEMORY;
	if (FAILED(hr)) return hr;

	hr = p_new->QueryInterface(riid, ppv);
	p_new->Release();

	return hr;
}
