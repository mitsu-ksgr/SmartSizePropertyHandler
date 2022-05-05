#include "CommandReceptor.h"


CommandReceptor::CommandReceptor()
	: mCommands({})
	, mExitRequest(false)
{
}

CommandReceptor::~CommandReceptor() {
}


//-----------------------------------------------------------------------------
//	Exit request
//-----------------------------------------------------------------------------
bool CommandReceptor::exitRequest() {
	return this->mExitRequest;
}

void CommandReceptor::requestExit() {
	this->mExitRequest = true;
}

void CommandReceptor::resetExitRequest() {
	this->mExitRequest = false;
}


//-----------------------------------------------------------------------------
//	Command methods
//-----------------------------------------------------------------------------
void localRequestExit() {}

int CommandReceptor::registerCommand(const std::wstring cmd, CmdFunc func)
{
	// Already registered.
	if (this->mCommands.find(cmd) != this->mCommands.end()) {
		return 1;
	}

	// Register command.
	this->mCommands.emplace(cmd, func);
	return 0;
}

int CommandReceptor::react(const std::wstring cmd)
{
	auto it = this->mCommands.find(cmd);

	// Command not found.
	if (it == this->mCommands.end()) {
		return 1;
	}

	if (it->second == &localRequestExit) {
		this->requestExit();
	} else {
		it->second();
	}
	return 0;
}

int CommandReceptor::registerExitCommands(std::initializer_list<std::wstring> args)
{
	int count = 0;
	for (auto&& cmd : args) {
		int ret = this->registerCommand(std::wstring(cmd), &localRequestExit);
		if (ret == 0) count++;
	}
	return count;
}
