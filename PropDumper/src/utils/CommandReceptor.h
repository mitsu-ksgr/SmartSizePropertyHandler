/*
 * CommandReceptor.h
 * 
 */

#ifndef __SRC_UTILS_CommandReceptor_H__
#define __SRC_UTILS_CommandReceptor_H__

#include <string>
#include <unordered_map>

typedef void (*CmdFunc)();

class CommandReceptor
{
public:
	CommandReceptor();
	~CommandReceptor();

	int registerCommand(const std::wstring cmd, CmdFunc func);
	int registerExitCommands(std::initializer_list<std::wstring> args);
	int react(const std::wstring cmd);

	bool exitRequest();
	void requestExit();
	void resetExitRequest();

private:
	bool mExitRequest;
	std::unordered_map<std::wstring, CmdFunc> mCommands;
};

#endif /* __SRC_UTILS_CommandReceptor_H__ */
