#pragma once

#include "AppConfig.h"

class AzureCommunicator
{
public:
	AzureCommunicator(const AppConfig& config_);
	void doJob();

private:
	const AppConfig config;

	void addFile();
	void deleteFile();
	void printLogs();
};

