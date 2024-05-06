#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include <temporary_file_handler/temporary_file_handler.hpp>

int main()
{
    std::string test_message = "Temporary File Handler:  This file will self distruct.";
    
    temporary_file_handler tfh;

    std::filesystem::path test_file = tfh.create_temporary_file();
    std::filesystem::path test_file_2 = tfh.create_temporary_file();
    std::filesystem::path test_file_3 = tfh.create_temporary_file();
    
    std::cout << "Temporary file path: " << test_file << "\n";
    
    std::ofstream ofs(test_file, std::ios::binary);
    ofs.write(test_message.c_str(), test_message.size());
    ofs.close();
    
    std::string message_from_file;

    if(std::filesystem::exists(test_file))
    {
        std::ifstream ifs(test_file, std::ios::binary);
        std::stringstream buffer;
        if(ifs) 
        {
            buffer << ifs.rdbuf();
            ifs.close();
        }
        message_from_file = buffer.str();
    } 
    else 
    {
        std::cerr << "Error opening file: " << test_file << "\n";
    }

    std::cout << "\n\nContents of temporary file:\n" << message_from_file << "\n";

    if(message_from_file == test_message)
    {
        std::cout << "\n\nSUCCESS: All Tests passed.\n";
    }
    else
    {
        std::cout << "\n\nFAILURE: An error occured creating temporary file.\n";
    }

    return 0;
}
