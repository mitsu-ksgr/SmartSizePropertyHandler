#include "SelectForm.h"

#include <conio.h>
#include <algorithm>
#include <iostream>


//-----------------------------------------------------------------------------
//  Utils
//-----------------------------------------------------------------------------
namespace {
    enum class Action {
        kActUp, kActDown, kActSelected
    };

    Action getAction() {
        constexpr int kEnter = 13;
        constexpr int kArrowUp = 72;
        constexpr int kArrowDown = 80;

        do {
            int ch = _getch();

            // * Detect Arrow key (Windows environment only!)
            // Windows's _getch returns two keycodes for arrow keys and Fn keys.
            // if arrow keys, it returns '0xE0' first followed by key code (Up=72, Down=80)
            // see: https://docs.microsoft.com/ja-jp/cpp/c-runtime-library/reference/getch-getwch?view=msvc-170#remarks
            if (ch == 0xE0) ch = _getch();

            switch (ch) {
            case 'k': case kArrowUp:
                return Action::kActUp;

            case 'j': case kArrowDown:
                return Action::kActDown;

            case kEnter:
                return Action::kActSelected;

            default:
                continue;
            }
        } while (true);
    }

    void eraseLines(size_t count) {
        if (count <= 0) return;

        // https://en.wikipedia.org/wiki/ANSI_escape_code#CSI_(Control_Sequence_Introducer)_sequences
        constexpr wchar_t kEraseLine[] = L"\x1B[2K";
        constexpr wchar_t kCursorUp[] = L"\x1B[1A";
        constexpr wchar_t kCR[] = L"\r"; // Carriage Return

        std::wcout << kEraseLine;
        for (unsigned int i = 1; i < count; ++i) {
            std::wcout << kCursorUp << kEraseLine;
        }
        std::wcout << kCR;
    }
}



//-----------------------------------------------------------------------------
//  SelectForm class
//-----------------------------------------------------------------------------
SelectForm::SelectForm(const std::vector<std::wstring> &ref, size_t max_lines)
	: mList(&ref)
    , mMaxDisplayLines(max_lines)
{
}

SelectForm::~SelectForm() {
	this->mList = nullptr;
}

void SelectForm::setMaxDisplaySize(size_t max_size) {
    this->mMaxDisplayLines = max_size;
}

size_t SelectForm::select() {
    const size_t len = this->mList->size();
    const size_t display_size = std::min(len, this->mMaxDisplayLines);
    const size_t is_scroll = (len > this->mMaxDisplayLines);
    const size_t non_visible_size = len - display_size;

    size_t offset = 0;
    size_t cursor = 0;

    do {
        size_t line_count = 0;

        // Show select list.
        if (is_scroll && offset > 0) {
            std::wcout << "   ..." << std::endl;
            line_count++;
        }
        for (size_t i = 0; i < display_size; ++i) {
            size_t idx = i + offset;
            std::wcout << " " << (idx == (offset + cursor) ? ">" : " ") << " ";
            std::wcout << this->mList->at(idx) << std::endl;
            line_count++;
        }
        if (is_scroll && offset < non_visible_size) {
            std::wcout << "   ..." << std::endl;
            line_count++;
        }

        // Input
        bool is_selected = false;
        switch (getAction()) {
        case Action::kActUp:
            if (cursor > 0) {
                cursor--;
            } else if (offset > 0) {
                offset--;
            }
            break;

        case Action::kActDown:
            if (offset + cursor < len - 1) {
                if (cursor + 1 >= display_size) {
                    offset++;
                } else {
                    cursor++;
                }
            }
            break;

        case Action::kActSelected:
            is_selected = true;
            break;
        }
        if (is_selected) break;

        // Erase select list for flash.
        eraseLines(line_count + 1);
    } while (true);

    return offset + cursor;
}

