/*
 * PropDescEnum.h
 *
 */

#include "PropDescEnumHelper.h"

namespace {
	// https://docs.microsoft.com/ja-jp/windows/win32/api/propsys/ne-propsys-propdesc_enumfilter
	static const std::unordered_map<PROPDESC_ENUMFILTER, std::wstring> kPropDescMap = {
		{ PDEF_ALL,				L"All" },
		{ PDEF_SYSTEM,			L"System" },
		{ PDEF_NONSYSTEM,		L"Non system" },
		{ PDEF_VIEWABLE,		L"Viewable" },
		{ PDEF_QUERYABLE,		L"Queryable" },
		{ PDEF_INFULLTEXTQUERY,	L"In full-text query" },
		{ PDEF_COLUMN,			L"Columns" },
	};

	static const std::vector<std::wstring> kPropDescsText = {
		L"All",
		L"System",
		L"Non system",
		L"Viewable",
		L"Queryable",
		L"In full-text query",
		L"Columns",
	};
}

const std::vector<std::wstring>&
PropDescEnumHelper::getPropDescTexts() {
	return kPropDescsText;
}

PROPDESC_ENUMFILTER PropDescEnumHelper::convertStrToPropDesc(std::wstring text)
{
	for (auto &it : kPropDescMap) {
		if (it.second == text) {
			return it.first;
		}
	}
	return PDEF_ALL;
}

std::wstring PropDescEnumHelper::convertPropDescToStr(PROPDESC_ENUMFILTER pd)
{
	return kPropDescMap.at(pd);
}

