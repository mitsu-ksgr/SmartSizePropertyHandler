/*
 * dll.cpp
 */
#include "dll.h"

#include <new>
#include <sstream>
#include <string>

#include <windows.h>

#include "SmartSizePropertyHandler.h"
#include "ClassFactory.h"


//-----------------------------------------------------------------------------
// Utils
//-----------------------------------------------------------------------------
namespace {
    std::wstring concatStrings(std::initializer_list<std::wstring> wstrs)
    {
        std::wostringstream os;
        for (auto &&s : wstrs) {
            os << s;
        }
        return os.str();
    }

    std::wstring getModuleFileName(HMODULE hModule)
    {
        wchar_t buf[MAX_PATH] = { L'\0' };
        unsigned long ret = GetModuleFileNameW(hModule, buf, MAX_PATH);
        if (ret == 0) return L"";

        return std::wstring(buf);
    }

    std::wstring convertGUIDtoString(REFCLSID clsid)
    {
        wchar_t buf[MAX_PATH];
        unsigned long ret = StringFromGUID2(clsid, buf, MAX_PATH);
        if (ret == 0) return L"";

        return std::wstring(buf);

    }

    /**
     * Sets the data for the specified value in the specified registry key and subkey.
     */
    HRESULT setValueToRegisterKey(
        const HKEY hkey, const wchar_t *regkey, const wchar_t *value
    ) {
        const std::wstring ws = std::wstring(value);
        const size_t len = ws.size() * sizeof(wchar_t);

        return HRESULT_FROM_WIN32(RegSetKeyValueW(
            hkey, regkey, L"", REG_SZ, ws.c_str(), static_cast<DWORD>(len)
        ));
    }

    HRESULT setValueToRegisterKey(
        const HKEY hkey, const wchar_t *regkey,
        const wchar_t *value_name, const wchar_t *value
    ) {
        const std::wstring ws = std::wstring(value);
        const size_t len = ws.size() * sizeof(wchar_t);

        return HRESULT_FROM_WIN32(RegSetKeyValueW(
            hkey, regkey, value_name, REG_SZ, ws.c_str(), static_cast<DWORD>(len)
        ));
        return S_OK;
    }
}



//-----------------------------------------------------------------------------
// Keys
//-----------------------------------------------------------------------------
namespace {
    constexpr const wchar_t *kSmartSizePropKey          = L"Software\\Classes\\CLSID\\" SMARTSIZE_PROP_GUID_W;
    constexpr const wchar_t *kSmartSizePropSubKey       = L"Software\\Classes\\CLSID\\" SMARTSIZE_PROP_GUID_W L"\\InprocServer32";
    constexpr const wchar_t *kSmartSizePropHandler      = L"Software\\Microsoft\\Windows\\CurrentVersion\\PropertySystem\\PropertyHandlers\\" SMARTSIZE_PROP_TARGET;
    constexpr const wchar_t *kSmartSizePropHandler_x64  = L"Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\PropertySystem\\PropertyHandlers\\" SMARTSIZE_PROP_TARGET;
} 
 

//-----------------------------------------------------------------------------
// Reference Manager
//-----------------------------------------------------------------------------
namespace {
    // Handle the DLL's module
    HMODULE g_hModule = nullptr;

    // Reference counter
    long g_cRefModule = 0;
}

void DllAddRef()
{
    InterlockedIncrement(&g_cRefModule);
}

void DllRelease()
{
    InterlockedDecrement(&g_cRefModule);
}


//-----------------------------------------------------------------------------
// DLL Functions
//-----------------------------------------------------------------------------
BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        DisableThreadLibraryCalls(hModule);
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


/**
 * Determines whether the DLL that implements this function is in use.
 * 
 * Ref: https://docs.microsoft.com/ja-jp/windows/win32/api/combaseapi/nf-combaseapi-dllcanunloadnow
 */
_Use_decl_annotations_
STDAPI DllCanUnloadNow(void)
{
    //DebugLog("dll.cpp: DllCanUnloadNow: Can? %s", (g_cRefModule == 0 ? "TRUE" : "FALSE"));
    // Only allow the DLL to be unloaded after all outstanding references have been released.
    return (g_cRefModule == 0) ? S_OK : S_FALSE;
}

/**
 * Retrieves the class object from a DLL object handler or object application.
 * 
 * Ref: https://docs.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-dllgetclassobject
 */
_Use_decl_annotations_
STDAPI DllGetClassObject(REFCLSID clsid, REFIID riid, void **ppv)
{
    *ppv = nullptr;

    if (clsid != __uuidof(SmartSizePropertyHandler)) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    IClassFactory *factory = new (std::nothrow) ClassFacotry(
        SmartSizePropertyHandler_CreateInstance
    );
    if (!factory) return E_OUTOFMEMORY;

    HRESULT hr = factory->QueryInterface(riid, ppv);
    factory->Release();

    return hr;
}

/**
 * Handles installation and setup for a DLL.
 *
 * Ref: https://docs.microsoft.com/ja-jp/windows/win32/api/shlwapi/nf-shlwapi-dllinstall
 */
//STDAPI DllInstall(BOOL bInstall, LPCWSTR pwszCmdLine)
//{
//    // bInstall is true ---> do DllRegisterServer()
//    //             false --> do DllUnregisterServer()
//    // pwszCmdLine is 'user' --> use HKEY_CURRENT_USER
//    //             not       --> use HKEY_LOCAL_MACHINE
//}


/**
 * Register COM Objects and Property Handler to the registry.
 * This method called when register the DLL of COM component to registry.
 * ex: `> regsvr32 foo.dll`
 */
STDAPI DllRegisterServer()
{
    DebugLog("dll.cpp: DllRegisterServer");

    std::wstring mod_name = getModuleFileName(g_hModule);
    if (mod_name == L"") return E_FAIL;

    HRESULT hr = S_OK;
    //REFCLSID clsid = __uuidof(SmartSizePropertyHandler);
    HKEY hkey_root = HKEY_LOCAL_MACHINE; // or HKEY_CURRENT_USER
    
    // Register: CLSID\\{uuid}
    hr = setValueToRegisterKey(
        hkey_root, kSmartSizePropKey, SMARTSIZE_PROP_DESCRIPTION
    );
    if (FAILED(hr)) return hr;

    // Register: CLSID\\{uuid}\\InprocServer32
    hr = setValueToRegisterKey(
        hkey_root, kSmartSizePropSubKey, mod_name.c_str()
    );
    if (FAILED(hr)) return hr;
    
    hr = setValueToRegisterKey(
        hkey_root, kSmartSizePropSubKey, L"ThreadingModel", L"Both"
    );
    if (FAILED(hr)) return hr;


    // Register: PropertyHandler
    hr = setValueToRegisterKey(
        HKEY_LOCAL_MACHINE, kSmartSizePropHandler, SMARTSIZE_PROP_GUID_W
    );
    if (FAILED(hr)) return hr;
    hr = setValueToRegisterKey(
        HKEY_LOCAL_MACHINE, kSmartSizePropHandler_x64, SMARTSIZE_PROP_GUID_W
    );
    if (FAILED(hr)) return hr;

    // Success
    return hr;
}

/**
 * Unregister COM Objects and Property Handler to the registry.
 * This method called when unregister the DLL of COM component to registry.
 * ex: `> regsvr32 /u foo.dll`
 */
STDAPI DllUnregisterServer()
{
    DebugLog("dll.cpp: DllUnregisterServer start");

    HRESULT hr = S_OK;
    const HKEY hkey_root = HKEY_LOCAL_MACHINE; // or HKEY_CURRENT_USER

    // Unregister COM Object
    hr = HRESULT_FROM_WIN32(RegDeleteTree(hkey_root, kSmartSizePropKey));
    if (FAILED(hr)) return hr;

    // Unregister Property Handler.
    hr = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_LOCAL_MACHINE, kSmartSizePropHandler));
    if (FAILED(hr)) return hr;
    hr = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_LOCAL_MACHINE, kSmartSizePropHandler_x64));
    if (FAILED(hr)) return hr;

    return hr;
}

