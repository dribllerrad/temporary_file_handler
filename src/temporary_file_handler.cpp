#include <temporary_file_handler.hpp>
#include <random>
#include <sstream>

temporary_file_handler::temporary_file_handler()
{
	temporary_directory_path = create_temporary_directory();
}

temporary_file_handler::~temporary_file_handler(){
	std::string text = "Deleting temporary directory... \n";
	std::string e = "echo -e ";
	std::string command = "exec ";
	system((command + e + text).c_str());
	std::filesystem::remove_all(temporary_directory_path);
}

std::filesystem::path temporary_file_handler::create_temporary_file()
{
	//std::filesystem::path temporary_directory_path = create_temporary_directory();
	std::random_device ran_dev;
	std::mt19937 randomizer(ran_dev());
	std::uniform_int_distribution<uint64_t> distribution(0);
	std::filesystem::path file_path;
	std::stringstream ss;
	ss << std::hex << distribution(randomizer);
	
	uint8_t max_tries = 100;
	uint8_t i = 0;
	while (true)
	{
		std::stringstream ss;
		ss << std::hex << distribution(randomizer);
		file_path = temporary_directory_path / ss.str();
		if (!std::filesystem::exists(file_path))
		{
			break;
		}
		if (i == max_tries)
		{
			throw std::runtime_error("Error: Unique file name could not be created.");
		}
		i++;
	}
	return file_path;
};

std::filesystem::path temporary_file_handler::create_temporary_directory()
{
	auto temp_path = std::filesystem::temp_directory_path();
	std::random_device ran_dev;
	std::mt19937 randomizer(ran_dev());
	std::uniform_int_distribution<uint64_t> distribution(0);
	std::filesystem::path path;

	uint8_t max_tries = 100;
	uint8_t i = 0;
	while (true)
	{
		std::stringstream ss;
		ss << std::hex << distribution(randomizer);
		path = temp_path / ss.str();
		if (std::filesystem::create_directory(path))
		{
			break;
		}
		if (i == max_tries)
		{
			throw std::runtime_error("Error: Unique directory name could not be created.");
		}
		i++;
	}
	return path;
}