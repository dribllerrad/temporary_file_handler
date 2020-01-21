
#pragma once

#include <filesystem>

class temporary_file_handler
{
public:
	temporary_file_handler();
	~temporary_file_handler();
	std::filesystem::path create_temporary_file();

private:
	std::filesystem::path temporary_directory_path;
	std::filesystem::path create_temporary_directory();
};
