# 🕵️‍♂️ USpy – USB Device Monitor

**USpy** is a  lightweight C++ console application designed for real-time USB device monitoring and automated file copying. Ideal for quick backups, forensic data collection, and seamless device management across Linux and Windows.

## Key Features

- **Real-time USB Monitoring:** Instantly detects USB insertion and removal events.
- **Automatic File Listing:** Displays contents of inserted USB drives.
- **Auto-Copy Function:** Copies files to your chosen destination folder automatically.
- **Cross-Platform:** Runs on both Linux (x86_64) and Windows (64-bit).
- **Minimal Footprint:** Lightweight, fast, and easy to use.
- **Convenient Controls:** Restart copying with `rs` command; exit anytime with `Ctrl+C`.


## Quick Start: Pre-Built Binaries

Effortlessly get started with ready-to-use binaries:


| Platform | Download Link |
| :-- | :-- |
| Linux (x86_64) | [USpy-linux-v1](https://github.com/yourusername/USpy/releases/download/v1.0.1/USpy-linux) |
| Windows (64-bit) | [USpy-win-v1.exe](https://github.com/yourusername/USpy/releases/download/v1.0.1/USpy-win.exe) |

**Linux Download Example:**

```bash
wget https://github.com/yourusername/USpy/releases/download/v1.0/USpy-linux
chmod +x USpy-linux
./USpy-linux
```


## Manual Installation

### Prerequisites

| Platform | Dependencies |
| :-- | :-- |
| Linux | `g++`, `libudev-dev` |
| Windows | MinGW-w64 or Visual Studio |

**Install on Linux:**

```bash
sudo apt update && sudo apt install g++ libudev-dev
```


## Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/USpy.git
cd USpy

# Build for Linux
make

# Cross-compile for Windows (from Linux)
make win

# Run tests
make test
```


## Usage Guide

1. **Navigate to the Target Directory:**

```bash
cd /path/where/you/want/files/copied
```

2. **Run USpy:**
    - **Linux:** `./USpy-linux`
    - **Windows:** `./USpy-win.exe`
3. **Insert a USB Device:** Monitor activity and let USpy handle the rest.

**Runtime Commands:**

- `rs` – Restart the copying process
- `Ctrl+C` – Exit the application


## Project Structure

```
usb-monitor/
├── Makefile
├── src/        # Core source files
├── include/    # Header files
├── tests/      # Test cases
└── dist/       # Compiled binaries
```


## Testing

```bash
# Run all tests
make test

# Test file utilities only
make test-utils

# Test a specific file
make test-f file=tests/test_file.cpp
```


## License

MIT License – Free for personal and commercial use. See [LICENSE](LICENSE) for details.

## Author

**Sahedul Islam Rony** <br/>
Email: sahedul.dev@gmail.com <br/>
GitHub: [@sahedulislamrony](https://github.com/sahedulislamrony)

> **Tip:** Launch USpy from your target directory to automatically copy files into date-stamped folders for easy organization.
