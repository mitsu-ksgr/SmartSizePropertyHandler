/*
 * PropList.h
 *
 */

#ifndef __SRC_PropList_H__
#define __SRC_PropList_H__

#include <string>
#include <vector>

#include <propsys.h>

// PropList Class
//
// Get PropertyDescription list.
//
class PropList
{
public:
	PropList(PROPDESC_ENUMFILTER filter);
	~PropList();

	bool isInitSucceeded();
	const std::vector<std::wstring> &getList();
	std::wstring at(size_t idx);
	std::vector<std::wstring> findByName(const std::wstring & word);

private:
	void init();

	HRESULT mHResult; // last HResult
	PROPDESC_ENUMFILTER mFilter;
	std::vector<std::wstring> mProps;
};


#endif /* __SRC_PropList_H__ */
