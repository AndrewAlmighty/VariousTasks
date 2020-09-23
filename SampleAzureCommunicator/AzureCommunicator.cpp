#include "AzureCommunicator.h"

#include <filesystem>
#include <iostream>

AzureCommunicator::AzureCommunicator(const AppConfig & config_) : config(config_)
{
}

void AzureCommunicator::doJob()
{
	AppConfig::Action action = config.whichActionToPerform();

	switch (action)
	{
	case AppConfig::Action::AddFile:
		addFile();
		break;
	case AppConfig::Action::DeleteFile:
		deleteFile();
		break;
	case AppConfig::Action::PrintLogs:
		printLogs();
		break;
	case AppConfig::Action::NotDefined:
		break;
	}
}

void AzureCommunicator::addFile()
{
	const std::string& file = config.getFile();
	std::cout << "Copying \"" << file << "\" to Azure service ... ";

	bool fileAdded = false;
	std::string err;
	std::filesystem::path filePath(config.getFile());

	// 1. Check if path points to directory
	if (std::filesystem::is_directory(filePath))
		err = "Got path to the directory: [" + file + "], not to file. App allows only to update single files!";

	// 2. Check if path points to existing file
	if (err.empty() && !std::filesystem::exists(std::filesystem::path(file)))
		err = "Could not find file: " + file;

	if (err.empty())
	{
		//3. Check if file doesnm't exceeds file limit.
		constexpr size_t FILE_MAX_SIZE = 2 * 1024 * 1024;	//2 MB file limit
		size_t fileSize = std::filesystem::file_size(filePath);

		if (fileSize > FILE_MAX_SIZE)
			err = "File: " + file + " exceeds file size limit. Max file size: " + std::to_string(FILE_MAX_SIZE) + ", file size: " + std::to_string(fileSize);
	}

	if (err.empty())
	{
		fileAdded = true;
	}

	if (fileAdded)
		std::cout << "OK\n";
	else
		std::cout << "Error";
}

void AzureCommunicator::deleteFile()
{
	std::cout << "Removing \n";
}

void AzureCommunicator::printLogs()
{
	std::cout << "Printing \n";
}
