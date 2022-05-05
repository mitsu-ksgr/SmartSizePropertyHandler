/*
 * SelectForm.h
 * 
 */

#ifndef __SRC_UTILS_SelectForm_H__
#define __SRC_UTILS_SelectForm_H__

#include <string>
#include <vector>

class SelectForm
{
public:
	static const size_t DefaultMaxDisplayLines = 10;

	SelectForm(
		const std::vector<std::wstring> &ref,
		size_t max_lines = DefaultMaxDisplayLines);
	~SelectForm();

	size_t select();
	void setMaxDisplaySize(size_t max_size);


private:
	const std::vector<std::wstring> *mList;
	size_t mMaxDisplayLines;
};

#endif /* __SRC_UTILS_SelectForm_H__ */
