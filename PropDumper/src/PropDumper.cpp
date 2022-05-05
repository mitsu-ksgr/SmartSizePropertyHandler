//
// PropDumper.cpp
// 
// Show Prop Description properties.
// 
// inspired by https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/appplatform/propertyschemas
//

#include <conio.h>
#include <iostream>
#include <string>

// windows headers
#include "combaseapi.h"
#include "propsys.h" // Need link: Propsys.lib

// PropDumper headers
#include "PropList.h"
#include "PropProp.h"
#include "PropDescEnumHelper.h"
#include "utils/CommandReceptor.h"
#include "utils/SelectForm.h"


//-----------------------------------------------------------------------------
//  Helpers
//-----------------------------------------------------------------------------
void showPropProp(std::wstring prop_name) {
    PropProp pp(prop_name);
    std::wcout << L"- Key                    : " << pp.key() << std::endl;
    std::wcout << L"- Canonical Name         : " << pp.canonicalName() << std::endl;
    std::wcout << L"- Display Name           : " << pp.displayName() << std::endl;
    std::wcout << L"- Edit Invitation        : " << pp.editInvitation() << std::endl;
    std::wcout << L"- Default Column Width   : " << pp.defaultColumnWidth() << std::endl;
    std::wcout << L"- Sort Description Label : " << pp.sortDescriptionLabel() << std::endl;
}


//-----------------------------------------------------------------------------
//  Commands
//-----------------------------------------------------------------------------
void cmdShowHelp() {
    std::wcout << std::endl << std::endl;
    std::wcout << L"* Prop Dumper" << std::endl;
    std::wcout << L"Dump PropertyDescription details." << std::endl;
    std::wcout << std::endl;

    std::wcout << L"Commands:" << std::endl;
    std::wcout << L"- help, usage\t\tshow help." << std::endl;
    std::wcout << L"- exit, quit, q\t\texit." << std::endl;
    std::wcout << L"- clear, c\t\tclear console." << std::endl;
    std::wcout << std::endl;
    std::wcout << L"- prop\t\t\tshow property details." << std::endl;
    std::wcout << L"- search\t\tsearch property." << std::endl;
    std::wcout << std::endl;
}

void cmdListProperties() {
    const auto &kPropDescs = PropDescEnumHelper::getPropDescTexts();
    SelectForm pd_form(kPropDescs);

    // PropDescription
    std::wcout << L"* Select property description:" << std::endl;
    size_t select = pd_form.select();
    std::wstring pd_text = kPropDescs.at(select);
    const auto pd_selected = PropDescEnumHelper::convertStrToPropDesc(pd_text);
    std::wcout << std::endl;

    // PropDescription Property
    PropList pl(pd_selected);
    if (pl.getList().size() == 0) {
        std::wcout << pd_text << L": No Properties." << std::endl;
        return;
    }

    SelectForm pl_form(pl.getList());
    std::wcout << L"* " << pd_text << L": Select property:" << std::endl;
    select = pl_form.select();
    std::wcout << std::endl;

    // PropProp
    std::wcout << std::endl;
    std::wcout << L"* " << pd_text << L" > " << pl.at(select) << std::endl;
    showPropProp(pl.at(select));
}

void cmdSearchProp() {
    std::wcout << std::endl;
    std::wcout << L"* Property Search" << std::endl;
    std::wcout << L"Seach word > ";

    std::wstring input;
    std::wcin >> input;

    PropList pl(PDEF_ALL);
    auto list = pl.findByName(input);
    std::wcout << "Find: " << list.size() << std::endl << std::endl;
    if (list.size() == 0) return;

    SelectForm form(list);
    size_t select = form.select();
    std::wcout << std::endl;

    // PropProp
    std::wcout << std::endl;
    std::wcout << L"* " << list.at(select) << std::endl;
    showPropProp(list.at(select));
}

void cmdClearConsole() {
    // https://en.wikipedia.org/wiki/ANSI_escape_code#CSI_(Control_Sequence_Introducer)_sequences
    constexpr wchar_t kClear[]       = L"\x1B[2J";
    constexpr wchar_t kResetCursor[] = L"\x1B[1;1H";
    std::wcout << kClear << kResetCursor;
}

void cmdTest() {
    std::wcout << L"test command" << std::endl;
}


//-----------------------------------------------------------------------------
//  Entry point
//-----------------------------------------------------------------------------
int startUI() {
    CommandReceptor receptor;
    receptor.registerExitCommands({ L"exit", L"quit", L"q" });
    receptor.registerCommand(L"help", &cmdShowHelp);
    receptor.registerCommand(L"usage", &cmdShowHelp);
    receptor.registerCommand(L"clear", &cmdClearConsole);
    receptor.registerCommand(L"c", &cmdClearConsole);
    receptor.registerCommand(L"test", &cmdTest);

    // main command
    receptor.registerCommand(L"prop", &cmdListProperties);
    receptor.registerCommand(L"search", &cmdSearchProp);

    cmdShowHelp();
    do {
        std::wstring input;
        std::wcout << std::endl << L"> ";
        std::wcin >> input;

        if (receptor.react(input) != 0) {
            std::wcout << L"Command not found: " << input << std::endl;
        }

        if (receptor.exitRequest()) break;
    } while (1);

    return 0;
}

int wmain(int argc, wchar_t *argv[])
{
    // Init
    std::wcout.imbue(std::locale(""));

    // Initialize the COM library for the current thread.
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        wprintf(L"CoInitializeEx failed with error: 0x%08x\n", hr);
        return 1;
    }

    // Start command interface.
    startUI();

    // Close the COM library on the current thread.
    CoUninitialize();
    return 0;
}
