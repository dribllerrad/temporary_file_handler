#ifndef _DEBUG
#define _DEBUG
#endif

#include <iostream>
#include <temporary_file_handler/temporary_file_handler.hpp>

int main()
{
	std::cout << "Creating tfh object...\n";
	temporary_file_handler tfh;
	return 0;
}