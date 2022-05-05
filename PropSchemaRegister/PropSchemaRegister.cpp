// PropSchemaRegister.cpp :
//

#include <iostream>

#include <propsys.h>    // Propsys.lib
#include <shlwapi.h>    // Shlwapi.lib
#include <windows.h>


//-----------------------------------------------------------------------------
//  Utils
//-----------------------------------------------------------------------------
struct ArgInfo {
    enum Mode {
        MODE_NONE,
        MODE_REGISTER,
        MODE_UNREGISTER,
    };

    Mode mode;
    std::wstring path;
    std::wstring msg;
};


ArgInfo getArgInfo(int argc, wchar_t *argv[])
{
    ArgInfo ai = { ArgInfo::Mode::MODE_NONE, L"", L""};

    if (argc < 3) {
        ai.msg = L"Arg missing!";
        return ai;
    }

    switch (argv[1][0]) {
    case 'r': ai.mode = ArgInfo::Mode::MODE_REGISTER; break;
    case 'u': ai.mode = ArgInfo::Mode::MODE_UNREGISTER; break;
    default: ai.msg = L"Invalid mode."; return ai;
    }

    ai.path = std::wstring(argv[2]);

    return ai;
}

bool validateFile(const std::wstring &path)
{
    if (!PathFileExistsW(path.c_str())) {
        std::wcout << L"Error: File not found. Path: " << path << std::endl;
        return false;
    }

    // check file extension.
    const wchar_t *p_ext = PathFindExtensionW(path.c_str());
    if (std::wcscmp(p_ext, L".propdesc") != 0) {
        std::wcout << "Error: File is not .propdesc file. Path: " << path << std::endl;
        return false;
    }

    return true;
}


//-----------------------------------------------------------------------------
//  Register Functions
//-----------------------------------------------------------------------------
bool registerSchema(const std::wstring &path)
{
    HRESULT hr = PSRegisterPropertySchema(path.c_str());

    if (FAILED(hr)) {
        std::wprintf(L"Error: PSRegisterPropertySchema failed for schema file %s with error: 0x%08x\n", path.c_str(), hr);
        return false;
    }

    return true;
}

bool unregisterSchema(const std::wstring &path)
{
    HRESULT hr = PSUnregisterPropertySchema(path.c_str());

    if (FAILED(hr)) {
        std::wprintf(L"Error: PSUnregisterPropertySchema failed for schema file %s with error: 0x%08x\n", path.c_str(), hr);
        return false;
    }

    return true;
}


//-----------------------------------------------------------------------------
//  Entry point
//-----------------------------------------------------------------------------
void usage()
{
	std::wcout << L"Usage: [MODE] [ARGUMENT]" << std::endl;
    std::wcout << L"Mode:" << std::endl;
    std::wcout << L" r ... Register mode. register specify schema." << std::endl;
    std::wcout << L" u ... Unregister mode. unregister specify schema." << std::endl;
    std::wcout << L"Argument:" << std::endl;
    std::wcout << L" filepath ... path to schema file." << std::endl;
}
int wmain(int argc, wchar_t *argv[])
{
    // Init
    std::wcout.imbue(std::locale(""));

    // Get/Check arguments
    ArgInfo ai = getArgInfo(argc, argv);
    if (ai.mode == ArgInfo::Mode::MODE_NONE) {
        std::wcout << "ai.msg  = " << ai.msg << std::endl;
        usage();
        return 1;
    }
    if (!validateFile(ai.path)) {
        usage();
        return 1;
    }

    // Initialize the COM library for the current thread.
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        wprintf(L"CoInitializeEx failed with error: 0x%08x\n", hr);
        return 1;
    }

    // Register / Unregister
    bool result = false;
    switch (ai.mode) {
    case ArgInfo::Mode::MODE_REGISTER:
        result = registerSchema(ai.path);
        break;

    case ArgInfo::Mode::MODE_UNREGISTER:
        result = unregisterSchema(ai.path);
        break;
    }

    // Close the COM library on the current thread.
    CoUninitialize();

    if (!result) return 1;
    return 0;
}

