#include "AppConfig.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

bool AppConfig::parseAndValidateCmdArgs(int argc, char * argv[])
{
	if (argc == 1 || std::strcmp(argv[1], "--help") == 0 || std::strcmp(argv[1], "-h") == 0)
	{
		printHelp();
		return false;
	}

	std::unordered_map<std::string, std::string> args;
	const std::unordered_set<std::string> argsWithoutValues{
		"--logs"
	};

	for (size_t i = 1; i < argc; ++i)
	{
		std::string argKey(argv[i]);

		if (argKey.substr(0, 2) != "--")
		{
			std::cout << "Arg: " + argKey + " doesn't match pattern for option. Did you forget \'--\' before argument?";
			return false;
		}

		std::string argVal;
		if (argsWithoutValues.find(argKey) == argsWithoutValues.end())
		{
			if (i + 1 > argc - 1)	//because first arg is filename, so -1
			{
				std::cout << "No value passed through command line for argument: " << argKey << std::endl;
				return false;
			}

			argVal = std::string(argv[++i]);
		}

		if (!args.emplace(argKey, argVal).second)
		{
			std::cout << "Arg: " << argKey << " is duplicated!\n";
			return false;
		}
	}

	if (args.find("--add") != args.end())
	{
		action = Action::AddFile;
		file = args["--add"];
		args.erase("--add");
	}

	if (args.find("--delete") != args.end())
	{
		if (action != Action::NotDefined)
		{
			std::cout << "Only one action can be performed at once!\n";
			return false;
		}

		action = Action::DeleteFile;
		file = args["--delete"];
		args.erase("--delete");
	}

	if (args.find("--logs") != args.end())
	{
		if (action != Action::NotDefined)
		{
			std::cout << "Only one action can be performed at once!\n";
			return false;
		}

		action = Action::PrintLogs;
		args.erase("--logs");
	}

	if (action == Action::NotDefined)
	{
		std::cout << "No action was choosen!\n";
		printHelp();
		return false;
	}

	if (!args.empty())
	{
		std::cout << "There are not valid arguments passed via command line to app: ";
		for (const auto[key, val] : args)
			std::cout << "[" << key << ":" << val << "] ";
		
		std::cout << std::endl;
		return false;
	}

	return true;
}

void AppConfig::printHelp() const
{
	std::cout <<
		"Available actions (only one can be used at once):\n"
		"--add <filepath>      -- add file to azure\n"
		"--delete <filename>   -- remove file from azure\n"
		"--logs                -- print logs\n"
		"-h or --help          -- print help\n";
}
