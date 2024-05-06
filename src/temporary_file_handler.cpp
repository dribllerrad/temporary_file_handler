#ifndef NDEBUG
#define _DEBUG
#endif

#include <temporary_file_handler/temporary_file_handler.hpp>
#include <random>
#include <sstream>

#ifdef _DEBUG
#include <iostream>
#endif

class temporary_file_handler::tfh_impl
{
public:
	std::filesystem::path temporary_directory_path;
	std::filesystem::path create_temporary_directory();
};

temporary_file_handler::temporary_file_handler()
  : tfh_impl_{std::make_unique<tfh_impl>()}
{
	tfh_impl_->temporary_directory_path = tfh_impl_->create_temporary_directory();

#ifdef _DEBUG
	std::cout << "Creating temporary directory: " << tfh_impl_->temporary_directory_path << "\n";
#endif // _DEBUG

}


temporary_file_handler::~temporary_file_handler()
{
#ifdef _DEBUG
	std::cout << "Deleting temporary directory: " << tfh_impl_->temporary_directory_path << "\n";
#endif // _DEBUG

	std::filesystem::remove_all(tfh_impl_->temporary_directory_path);
}

std::filesystem::path temporary_file_handler::create_temporary_file()
{
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
		file_path = tfh_impl_->temporary_directory_path / ss.str();
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

std::filesystem::path temporary_file_handler::tfh_impl::create_temporary_directory()
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