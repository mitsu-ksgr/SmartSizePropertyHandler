#include "PropList.h"

#include <iostream>

PropList::PropList(PROPDESC_ENUMFILTER filter)
    : mHResult(0)
    , mFilter(filter)
    , mProps({})
{
    this->init();
}

PropList::~PropList() {
}

bool PropList::isInitSucceeded() {
    return SUCCEEDED(this->mHResult);
}

const std::vector<std::wstring>& PropList::getList() {
    return this->mProps;
}

std::wstring PropList::at(size_t idx) {
    return this->mProps.at(idx);
}

void PropList::init() {
    IPropertyDescriptionList *list;

    this->mHResult = PSEnumeratePropertyDescriptions(this->mFilter, IID_PPV_ARGS(&list));
    if (FAILED(this->mHResult)) return;

    // Get property count
    unsigned int prop_count = 0;
    this->mHResult = list->GetCount(&prop_count);
    if (FAILED(this->mHResult) || prop_count == 0) {
        list->Release();
        return;
    }

    // Get PropDescriptions
    for (unsigned int i = 0; i < prop_count; ++i) {
        IPropertyDescription *pd = nullptr;
        wchar_t *canonical_name = nullptr;

        do {
            this->mHResult = list->GetAt(i, IID_PPV_ARGS(&pd));
            if (FAILED(this->mHResult)) break;

            this->mHResult = pd->GetCanonicalName(&canonical_name);
            if (FAILED(this->mHResult)) break;
            if (canonical_name == nullptr) break;

            this->mProps.push_back(std::wstring(canonical_name));
        } while (false);

        if (canonical_name) CoTaskMemFree(canonical_name);
        if (pd) pd->Release();
        if (FAILED(this->mHResult)) break;
    }

    list->Release();
}

std::vector<std::wstring> PropList::findByName(const std::wstring & word)
{
    std::vector<std::wstring> ret = {};

    for (auto prop : this->mProps) {
        if (prop.find(word) == std::string::npos) continue;

        ret.push_back(std::wstring(prop));
    }

    return ret;
}
