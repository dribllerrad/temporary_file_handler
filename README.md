
# Temporary File Handler

## Overview
The Temporary File Handler is a cross-platform library designed to create temporary files that are automatically deleted. It ensures that sensitive data handled within temporary files remains secure and is cleaned up without manual intervention.

## Features
- **Cross-platform compatibility**: Works on various operating systems.
- **Automatic cleanup**: Files are removed when the object is destroyed or the program exits.
- **Security**: Implements obfuscation of filenames to protect the contents of temporary files.

## Getting Started

### Prerequisites
> [!WARNING]
- Ensure you have a compatible compiler for C++17 or higher.
- CMake version 3.15 or higher.

## Building and linking to the project
The project is designed to be easily integrated into another CMake project simply by adding this repo as a subdirectory and linking it to your main target.
```cmake
add_subdirectory(temporary_file_handler)
target_link_libraries(${YOUR_MAIN_TARGET} PUBLIC temporary_file_handler::temporary_file_handler)
```


However, it can easily be built and installed as a standalone library following the steps below:
1. Clone the repository:
   ```bash
   git clone https://github.com/dribllerrad/temporary_file_handler.git
   ```

2. Navigate to the project and create a build directory:  
   ```bash
   cd temporary_file_handler
   mkdir build
   cd build
   ```

3. Build with standard CMake:
   ```bash
   cmake .. && cmake --build .
   ```


4. Install the library:
   ```bash
   sudo cmake --install .
   ```
   > [!NOTE]  
   > sudo may be required for writing to standard dev locations such as /usr/local/lib and /usr/local/include. If you do not have sudo access, you can specify a custom install location with the CMAKE_INSTALL_PREFIX flag in step 3.

   This will install the library itself to your system's default library location.  It also installs the c++ header file to your system's default include location.  Finally it installs the generated cmake config files in the proper location so that you can use find_package to locate the library in your own projects.

   CMake itself does not have built in functionality for uninstalling targets so the library generates it's own uninstall target and script. To uninstall the library, navigate to the build directory and run:
   ```bash
   sudo cmake --build . --target uninstall
   ```
   This will remove the library, the header file, and the lib's cmake config files from your system.  Because CMake uses another build system under the hood, you can also use that build systems uninstall command directly.  
   Example: 
   ```bash
   sudo ninja uninstall
   ```
   or
   ```bash
   sudo make uninstall
   ```





## Usage Examples
### Creating a Temporary File
```cpp
// This is the only include needed for the library.
#include <temporary_file_handler/temporary_file_handler.hpp>
// If the library is either installed or used as a subdirectory with CMake, the header will be resolved correctly with either quotes or angle brackets.

// The following includes are only needed for the example.
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

int main() {
    
    // Createing the object sets up the an obfuscated file directory.  This entire directory will be deleted when the object goes out of scope.
    temporary_file_handler tfh;
    
    // Set up a temporary file path to write to using the method create_temporary_file().
    std::filesystem::path test_file = tfh.create_temporary_file();
    
    //  Any type of data can be used written to the file.
    std::string test_message = "Hello temporary file.";
    std::ofstream ofs(test_file, std::ios::binary);
    ofs.write(test_message.c_str(), test_message.size());
    ofs.close();

    // Use the file
    std::ifstream ifs(test_file, std::ios::binary);
    std::stringstream buffer;
    if(ifs) 
    {
        buffer << ifs.rdbuf();
        ifs.close();
        std::cout << buffer.str() << "\n"; 
    }

    // Any new file created will be managed by the library.
    std::filesystem::path test_file_2 = tfh.create_temporary_file();
    std::filesystem::path test_file_3 = tfh.create_temporary_file();

    // The enclosing directory and all managed files are automatically deleted when the object goes out of scope or the program exits without error.
    
    return 0;
}
```
> [!NOTE]  
> When the library is compiled in debug mode, the obfuscated names of the temporary directory path and any file creations will be printed to the console. This is useful for debugging purposes but is fully suppressed in Release builds.



## Contributing
Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated.

## License
Distributed under the MIT License. See LICENSE for more information.


Project Link: https://github.com/dribllerrad/temporary_file_handler

