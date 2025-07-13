<h1 align="center">USpy</h1>
<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Platforms: Linux | Windows](https://img.shields.io/badge/Platforms-Linux%20%7C%20Windows-blue)

</div>

USpy is a lightweight C++ console application for real-time USB device monitoring and automated file copying from inserted drives. Suitable for quick backups and forensic data collection.

## Features

- Real-time USB device monitoring (insertion/removal)
- Automatic file listing of USB contents
- Auto-copy files to destination folder
- Cross-platform support (Linux/Windows)
- Lightweight console application
- Restart copying with simple command (`rs`)

## Download Pre-built Binaries

Pre-built releases are available for quick setup:

| Platform         | Download Link                                                                                 |
| ---------------- | --------------------------------------------------------------------------------------------- |
| Linux (x86_64)   | [USpy-linux-v1.0](https://github.com/yourusername/USpy/releases/download/v1.0/USpy-linux)     |
| Windows (64-bit) | [USpy-win-v1.0.exe](https://github.com/yourusername/USpy/releases/download/v1.0/USpy-win.exe) |

### Linux download example

```bash
wget https://github.com/yourusername/USpy/releases/download/v1.0/USpy-linux
chmod +x USpy-linux
./USpy-linux
```

## Manual Installation

### Prerequisites

| Platform | Dependencies               |
| -------- | -------------------------- |
| Linux    | `g++`, `libudev-dev`       |
| Windows  | MinGW-w64 or Visual Studio |

**Linux setup:**

```bash
sudo apt update && sudo apt install g++ libudev-dev
```

## Building from Source

```bash
# Clone repository
git clone https://github.com/yourusername/USpy.git
cd USpy

# Build for Linux
make

# Cross-compile for Windows (from Linux)
make win

# Run tests
make test
```

## Usage

```bash
# 1. Navigate to your target directory
cd /path/where/you/want/files/copied

# 2. Run USpy (use downloaded binary or built one)
./USpy-linux      # Linux
./USpy-win.exe    # Windows

# 3. Insert USB device and monitor activity
```

**Runtime commands:**

- `rs` - Restarts copying process
- `Ctrl+C` - Exit the application

## Project Structure

```
usb-monitor/
├── Makefile
├── src/               # Core source files
├── include/           # Header files
├── tests/             # Test cases
└── dist/              # Compiled binaries
```

## Testing

````bash
# Run all tests
make test

# Test file utilities only
make test-utils

# Test specific file
```make
make test-f file=tests/test_file.cpp
````

## License

MIT License - Free for personal and commercial use. See [LICENSE](LICENSE) for details.

## Author

Sahedul Islam Rony  
CSE Undergraduate  
sahedul.dev@gmail.com

[![GitHub](https://img.shields.io/badge/GitHub-Profile-blue?logo=github)](https://github.com/sahedulislamrony)

**Tip:** Run USpy from your target directory to automatically copy files there with date-stamped folders.
