#include "AzureCommunicator.h"

#include <filesystem>
#include <iostream>

#include <was/storage_account.h>
#include <was/blob.h>
#include <was/table.h>
#include <cpprest/filestream.h>  
#include <cpprest/containerstream.h> 

const utility::string_t ACC_NAME = L"example_account";
const utility::string_t ACC_KEY = L"example_key";
const utility::string_t CONTAINER_NAME = L"example_container";
const utility::string_t TABLE_NAME = L"example_table";

const utility::string_t CONNECTION_STRING = L"DefaultEndpointsProtocol=https;AccountName=" + ACC_NAME + L";AccountKey=" + ACC_KEY;
const utility::string_t PROPERTY_ACTION = L"Action";
const utility::string_t PROPERTY_SIZE = L"Size";
const utility::string_t PROPERTY_DATE = L"Date";

AzureCommunicator::AzureCommunicator(const AppConfig& config_) : config(config_)
{
}

void AzureCommunicator::doJob()
{
	AppConfig::Action action = config.whichActionToPerform();

	switch (action)
	{
	case AppConfig::Action::Add:
		addFile();
		break;
	case AppConfig::Action::Delete:
		deleteFile();
		break;
	case AppConfig::Action::Print:
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
	size_t fileSize = static_cast<size_t>(std::filesystem::file_size(filePath));


	// 1. Check if path points to directory
	if (std::filesystem::is_directory(filePath))
		err = "Got path to the directory: [" + file + "], not to file. App allows only to update single files!";

	// 2. Check if path points to existing file
	if (err.empty() && !std::filesystem::exists(std::filesystem::path(file)))
		err = "Could not find file: " + file;

	if (err.empty())
	{
		//3. Check if file doesn't exceeds file limit.
		constexpr size_t FILE_MAX_SIZE = 2 * 1024 * 1024;	//2 MB file limit

		if (fileSize > FILE_MAX_SIZE)
			err = "File: " + file + " exceeds file size limit. Max file size: " + std::to_string(FILE_MAX_SIZE) + ", file size: " + std::to_string(fileSize);
	}

	if (err.empty())
	{
		try {
			//4. Send file
			azure::storage::cloud_storage_account storage_account = azure::storage::cloud_storage_account::parse(CONNECTION_STRING);
			azure::storage::cloud_blob_client blob_client = storage_account.create_cloud_blob_client();
			azure::storage::cloud_blob_container container = blob_client.get_container_reference(CONTAINER_NAME);
			utility::string_t pathToFile = utility::conversions::to_string_t(file);
			azure::storage::cloud_block_blob blockBlob = container.get_block_blob_reference(pathToFile);
			blockBlob.upload_from_file(pathToFile);

			//5. Update table
			azure::storage::cloud_table_client table_client = storage_account.create_cloud_table_client();
			azure::storage::cloud_table table = table_client.get_table_reference(TABLE_NAME);
			utility::string_t filename = utility::conversions::to_string_t(filePath.filename().string());
			azure::storage::table_entity addedFile(L"ADD", filename);
			azure::storage::table_entity::properties_type& properties = addedFile.properties();
			properties.reserve(3);
			properties[PROPERTY_SIZE] = azure::storage::entity_property(static_cast<int64_t>(fileSize));
			properties[PROPERTY_DATE] = azure::storage::entity_property(utility::datetime::utc_now());
			properties[PROPERTY_ACTION] = azure::storage::entity_property(L"File added");
			azure::storage::table_operation insert_operation = azure::storage::table_operation::insert_entity(addedFile);
			azure::storage::table_result insert_result = table.execute(insert_operation);
			fileAdded = true;
		}
		catch (std::exception& ex) {
			std::ignore = ex;
		}		
	}

	if (fileAdded)
		std::cout << "OK\n";
	else
		std::cout << "Error\n";
}

void AzureCommunicator::deleteFile()
{
	const std::string& file = config.getFile();
	std::cout << "Delete \"" << file << "\" to Azure service ... ";
	bool fileDeleted = false;

	try {
		// 1.First delete file.
		azure::storage::cloud_storage_account storage_account = azure::storage::cloud_storage_account::parse(CONNECTION_STRING);
		azure::storage::cloud_blob_client blob_client = storage_account.create_cloud_blob_client();
		azure::storage::cloud_blob_container container = blob_client.get_container_reference(CONTAINER_NAME);
		azure::storage::cloud_block_blob blockBlob = container.get_block_blob_reference(utility::conversions::to_string_t(file));
		blockBlob.delete_blob();
		fileDeleted = true;

		// 2. Add log to table
		azure::storage::cloud_table_client table_client = storage_account.create_cloud_table_client();
		azure::storage::cloud_table table = table_client.get_table_reference(TABLE_NAME);
		azure::storage::table_entity removedFile(L"DELETE", utility::conversions::to_string_t(file));
		azure::storage::table_entity::properties_type& properties = removedFile.properties();
		properties.reserve(2);
		properties[PROPERTY_DATE] = azure::storage::entity_property(utility::datetime::utc_now());
		properties[PROPERTY_ACTION] = azure::storage::entity_property(L"File removed");
		azure::storage::table_operation insert_operation = azure::storage::table_operation::insert_entity(removedFile);
		azure::storage::table_result insert_result = table.execute(insert_operation);
	}
	catch (std::exception& ex) {
		std::ignore = ex;
	}


	if (fileDeleted)
		std::cout << "OK\n";
	else
		std::cout << "Error\n";
}

void AzureCommunicator::printLogs()
{
	try {
		azure::storage::cloud_storage_account storage_account = azure::storage::cloud_storage_account::parse(CONNECTION_STRING);
		azure::storage::cloud_table_client table_client = storage_account.create_cloud_table_client();
		azure::storage::cloud_table table = table_client.get_table_reference(TABLE_NAME);
		azure::storage::table_query query;
		azure::storage::table_query_iterator it = table.execute_query(query);
		azure::storage::table_query_iterator end_of_results;

		for (; it != end_of_results; ++it)
		{
			const azure::storage::table_entity::properties_type& properties = it->properties();

			std::wcout << properties.at(PROPERTY_ACTION).string_value() << L" - " << it->row_key()
				<< L", action date: " << properties.at(PROPERTY_DATE).datetime_value().to_string();

			if (properties.find(PROPERTY_SIZE) != properties.end())
				std::wcout << L" - file size: " << properties.at(PROPERTY_SIZE).int64_value() << L" bytes";

			std::wcout << std::endl;
		}
	}
	catch (std::exception& ex) {
		std::ignore = ex;
		std::cout << "Loading logs error.";
	}
}
