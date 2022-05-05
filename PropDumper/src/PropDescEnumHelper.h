/*
 * PropDescEnumHelper.h
 *
 */

#ifndef __SRC_PropDescEnum_H__
#define __SRC_PropDescEnum_H__

#include <string>
#include <unordered_map> 
#include <vector>

#include <propsys.h>

// PropDescEnum Class
//
// PROPDESC_ENUMFILTER wrapper
//
class PropDescEnumHelper
{
public:
	static const std::vector<std::wstring>& getPropDescTexts();
	static PROPDESC_ENUMFILTER convertStrToPropDesc(std::wstring text);
	static std::wstring convertPropDescToStr(PROPDESC_ENUMFILTER pd);
};

#endif /* __SRC_PropDescEnum_H__ */
