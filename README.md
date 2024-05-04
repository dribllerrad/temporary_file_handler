
# Temporary File Handler

## Overview
The Temporary File Handler is a cross-platform library designed to create temporary files that are automatically deleted. It ensures that sensitive data handled within temporary files remains secure and is cleaned up without manual intervention.

## Features
- **Cross-platform compatibility**: Works on various operating systems.
- **Automatic cleanup**: Files are removed when the object is destroyed or the program exits.
- **Security**: Implements obfuscation to protect the contents of temporary files.

## Getting Started

### Prerequisites
- Ensure you have a compatible compiler for C++17 or higher.
- CMake version 3.15 or higher.

### Building the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/dribllerrad/temporary_file_handler.git

2. Navigate to the project directory:
cd temporary_file_handler

3. Create a build directory and navigate into it:
mkdir build && cd build

4. Run CMake and build the project:
cmake .. && cmake --build .

## Usage Examples
### Creating a Temporary File
```cpp
#include "temporary_file_handler.hpp"

int main() {
    // Create a temporary file
    TemporaryFileHandler tempFile;

    // Use the file
    // ...

    // File is automatically deleted when the object goes out of scope
    return 0;
}
```

### Customizing File Parameters
```cpp
#include "temporary_file_handler.hpp"

int main() {
    // Create a temporary file with custom parameters
    TemporaryFileHandler tempFile("custom_prefix_", "custom_suffix");

    // Use the file
    // ...

    // File is automatically deleted when the object goes out of scope
    return 0;
}
```

## Contributing
Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated.

## License
Distributed under the MIT License. See LICENSE for more information.


Project Link: https://github.com/dribllerrad/temporary_file_handler

