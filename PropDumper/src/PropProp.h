/*
 * PropProp.h
 *
 */

#ifndef __SRC_PropProp_H__
#define __SRC_PropProp_H__

#include <string>

#include <propsys.h>


// PropProp Class
//
// Get PropertyDescription's properties.
//
class PropProp
{
public:
	PropProp(std::wstring prop_name);
	~PropProp();

	HRESULT getHResult() {
		return this->mHResult;
	}
	std::wstring key() {
		return this->mKey;
	}
	std::wstring canonicalName() {
		return this->mCanonicalName;
	}
	std::wstring displayName() {
		return this->mDisplayName;
	}
	std::wstring editInvitation() {
		return this->mEditInvitation;
	}
	unsigned int defaultColumnWidth() {
		return this->mDefaultColumnWidth;
	}
	std::wstring sortDescriptionLabel() {
		return this->mSortDescriptionLabel;
	}

private:
	void init();
	void destroy();

	void initKey();
	void initCanonicalName();
	void initDisplayName();
	void initEditInvitation();
	void initDefaultColumnWidth();
	void initSortDescriptionLabel();

	HRESULT mHResult; // last HResult
	IPropertyDescription *mPropDesc;
	std::wstring mKey;
	std::wstring mCanonicalName;
	std::wstring mDisplayName;
	std::wstring mEditInvitation;
	unsigned int mDefaultColumnWidth;
	std::wstring mSortDescriptionLabel;
};


#endif /* __SRC_PropProp_H__ */
