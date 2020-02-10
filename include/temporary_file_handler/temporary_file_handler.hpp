
#pragma once

#include <filesystem>
#include <memory>

class temporary_file_handler
{
public:
	temporary_file_handler();
	~temporary_file_handler();
	std::filesystem::path create_temporary_file();

private:
	class tfh_impl;
	std::unique_ptr<tfh_impl> tfh_impl_;

	
};
