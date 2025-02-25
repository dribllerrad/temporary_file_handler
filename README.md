# Temporary File Handler

<!--
[![Build Status](https://github.com/dribllerrad/temporary_file_handler/workflows/CI/badge.svg)]
-->  

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)]

## Overview
The Temporary File Handler is a cross-platform library designed to create temporary files that are automatically deleted. It ensures that sensitive data handled within temporary files remains secure by obfuscating filenames.

## Features
- 🚀 **Cross-platform compatibility**: Works on various operating systems.
- 🧹 **Automatic cleanup**: Files are removed when the object is destroyed or the program exits.
- 🔒 **Security**: Implements obfuscation of filenames to protect the contents of temporary files.

## Getting Started

### Prerequisites
- [C++17](https://en.cppreference.com/w/cpp/compiler_support)
- [CMake 3.15+](https://cmake.org/download/)

> Ensure you have a compatible compiler for C++17 or higher and CMake version 3.15 or higher.

## Building and Installation

### Cloning the Repository
```bash
git clone https://github.com/dribllerrad/temporary_file_handler.git
```

### Building with CMake
```bash
cd temporary_file_handler
mkdir build
cd build
cmake .. && cmake --build .
```

### Installing the Library
```bash
sudo cmake --install .
```

> Note: Use `sudo` for writing to standard locations like `/usr/local/lib` and `/usr/local/include`. For custom install locations, use `CMAKE_INSTALL_PREFIX`.

## Usage Examples

### Creating a Temporary File
```cpp
#include <temporary_file_handler/temporary_file_handler.hpp>
// Additional includes for the example
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

int main() {
    temporary_file_handler tfh;
    std::filesystem::path test_file = tfh.create_temporary_file();
    std::string test_message = "Hello temporary file.";
    std::ofstream ofs(test_file, std::ios::binary);
    ofs.write(test_message.c_str(), test_message.size());
    ofs.close();

    std::ifstream ifs(test_file, std::ios::binary);
    std::stringstream buffer;
    if(ifs) {
        buffer << ifs.rdbuf();
        ifs.close();
        std::cout << buffer.str() << "\n";
    }

    return 0;
}
```

> Note: In debug mode, obfuscated names of temporary files are printed to the console.

<!--
## Contributing

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated.
   
Please read [CONTRIBUTING.md](https://github.com/dribllerrad/temporary_file_handler/blob/master/CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests.
-->  

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Project Link

- [GitHub Repository](https://github.com/dribllerrad/temporary_file_handler)
- [Documentation](https://github.com/dribllerrad/temporary_file_handler/wiki)
- [Issue Tracker](https://github.com/dribllerrad/temporary_file_handler/issues)
