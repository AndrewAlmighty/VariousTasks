#include "AppConfig.h"
#include "AzureCommunicator.h"

int main(int argc, char* argv[])
{
	AppConfig config;
	if (!config.parseAndValidateCmdArgs(argc, argv))
		return 0;

	AzureCommunicator communicator(config);
	communicator.doJob();
	return 0;
}