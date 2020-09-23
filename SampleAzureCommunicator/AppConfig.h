#pragma once

#include <string>

class AppConfig
{
public:
	enum Action
	{
		NotDefined = 0,
		AddFile = 1,
		DeleteFile = 2,
		PrintLogs = 3
	};

	AppConfig() = default;

	bool parseAndValidateCmdArgs(int argc, char* argv[]);
	void printHelp() const;

	Action whichActionToPerform() const { return action; }
	const std::string& getFile() const { return file; }

private:
	std::string file;
	Action action = Action::NotDefined;
};

