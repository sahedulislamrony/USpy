# USB Monitor - Cross Platform (C++)

A lightweight C++ console application that monitors USB insertions and removals in real time on both **Windows 11** and **Linux** systems.

## Features

- Detects USB device insertion/removal events
- Automatically identifies mount points or drive letters
- Lists all files and folders on the inserted USB device
- Supports Linux (via libudev) and Windows (via Windows API)
- Clean, modular code structure

## Prerequisites

### Linux

- g++ compiler with C++17 support
- libudev development library (`libudev-dev`)

Install dependencies on Debian/Ubuntu:

```bash
sudo apt update
sudo apt install g++ libudev-dev
```

### Windows

- MinGW-w64 cross-compiler or native MinGW on Windows
- Alternatively, Visual Studio for manual compilation

## Build Instructions

### Linux

```bash
make
```

Builds the Linux executable at `dist/linux`.

### Windows (Cross-compile on Linux)

```bash
make win
```

Builds the Windows executable at `dist/win.exe`.

### Windows (Native Build)

Use Visual Studio or MinGW on Windows to compile manually.

## Run Instructions

### Linux

```bash
./dist/linux
```

### Windows

```bash
./dist/win.exe
```

### Enabling Console Logging

By default, the application logs messages to a file named `usb_monitor.log` with minimal console output.

To enable full logging output to the console as well, set the environment variable `USB_MONITOR_LOG_CONSOLE` to `1` before running the application.

You can also set the log file path by setting the environment variable `USB_MONITOR_LOG_FILE` to the desired file path.

For example, on Linux/macOS:

```bash
export USB_MONITOR_LOG_CONSOLE=1
export USB_MONITOR_LOG_FILE=/path/to/your_log_file.log
./dist/linux
```

On Windows (PowerShell):

```powershell
$env:USB_MONITOR_LOG_CONSOLE = "1"
$env:USB_MONITOR_LOG_FILE = "C:\\path\\to\\your_log_file.log"
.\dist\win.exe
```

This will show all log messages in the console in addition to writing them to the specified log file.

## Testing

### Run all tests

```bash
make test
```

### Run utility tests

```bash
make test-utils
```

### Run a single test file

```bash
make test-f file=path/to/test_file.cpp
```

## Cleaning Build Artifacts

```bash
make clean
```

## Notes

- On Linux, ensure your system auto-mounts USB devices (GNOME/KDE usually handle this).
- On Windows, the app listens to system device events using `WM_DEVICECHANGE`.

## License

MIT License – free for personal and commercial use.
