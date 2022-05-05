#include "PropProp.h"


PropProp::PropProp(std::wstring prop_name)
    : mPropDesc(nullptr)
    , mKey(L"")
    , mCanonicalName(L"")
    , mDisplayName(L"")
    , mEditInvitation(L"")
    , mDefaultColumnWidth(0)
    , mSortDescriptionLabel(L"")
{
    this->mHResult = PSGetPropertyDescriptionByName(
        prop_name.c_str(), IID_PPV_ARGS(&this->mPropDesc));
    if (SUCCEEDED(this->mHResult)) {
        this->init();
    }
}

PropProp::~PropProp() {
    this->destroy();
}

void PropProp::destroy() {
    if (this->mPropDesc) {
        this->mPropDesc->Release();
        this->mPropDesc = nullptr;
    }
}


//-----------------------------------------------------------------------------
// Private Methods
//-----------------------------------------------------------------------------
void PropProp::init() {
	if (!this->mPropDesc) {
		return;
	}

    this->initKey();
    this->initCanonicalName();
    this->initDisplayName();
    this->initEditInvitation();
    this->initDefaultColumnWidth();
    this->initSortDescriptionLabel();
}

void PropProp::initKey() {
    PROPERTYKEY key;

    this->mHResult = this->mPropDesc->GetPropertyKey(&key);
    if (FAILED(this->mHResult)) return;

    wchar_t wc_key[PKEYSTR_MAX + 1] = { 0 };
    this->mHResult = PSStringFromPropertyKey(key, wc_key, ARRAYSIZE(wc_key));
    if (FAILED(this->mHResult)) return;

    this->mKey = std::wstring(wc_key);
}

void PropProp::initCanonicalName() {
    wchar_t *value = nullptr;
    this->mHResult = this->mPropDesc->GetCanonicalName(&value);
    if (FAILED(this->mHResult) || value == nullptr) return;

    this->mCanonicalName = std::wstring(value);
    CoTaskMemFree(value);
}

void PropProp::initDisplayName() {
    wchar_t *value = nullptr;
    this->mHResult = this->mPropDesc->GetDisplayName(&value);
    if (FAILED(this->mHResult) || value == nullptr) return;

    this->mDisplayName = std::wstring(value);
    CoTaskMemFree(value);
}

void PropProp::initEditInvitation() {
    wchar_t *value = nullptr;
    this->mHResult = this->mPropDesc->GetEditInvitation(&value);
    if (FAILED(this->mHResult) || value == nullptr) return;

    this->mEditInvitation = std::wstring(value);
    CoTaskMemFree(value);
}

void PropProp::initDefaultColumnWidth() {
    this->mHResult = this->mPropDesc->GetDefaultColumnWidth(&this->mDefaultColumnWidth);
    if (FAILED(this->mHResult)) {
        return;
    }
}

void PropProp::initSortDescriptionLabel() {
    wchar_t *ascend = nullptr;
    wchar_t *descend = nullptr;

    do {
        this->mHResult = this->mPropDesc->GetSortDescriptionLabel(FALSE, &ascend);
        if (FAILED(this->mHResult) || ascend == nullptr) break;

        this->mHResult = this->mPropDesc->GetSortDescriptionLabel(TRUE, &descend);
        if (FAILED(this->mHResult) || descend == nullptr) break;

        this->mSortDescriptionLabel =
            std::wstring(ascend) +
            L" / " +
            std::wstring(descend);
    } while (0);

    CoTaskMemFree(ascend);
    CoTaskMemFree(descend);
}
