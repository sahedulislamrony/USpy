# USB Monitor Developer Documentation

## Project Overview

USB Monitor is a lightweight C++ console application that monitors USB device insertions and removals in real time on both Linux and Windows platforms. It automatically identifies the mount point or drive letter and lists all files and folders on the inserted USB device.

---

## Prerequisites and Dependencies

### Linux

- g++ compiler with C++17 support
- libudev development library (`libudev-dev`)

Install dependencies on Debian/Ubuntu:

```bash
sudo apt update
sudo apt install g++ libudev-dev
```

### Windows

- MinGW-w64 cross-compiler (for building on Linux targeting Windows) or native MinGW on Windows
- Alternatively, Visual Studio can be used for manual compilation
- Windows API libraries: user32, gdi32 (linked automatically in Makefile)

---

## Build Instructions

### Linux

1. Open a terminal in the project root directory.
2. Run the following command to build the Linux executable:
   ```bash
   make
   ```
   This will compile the source files and create the executable at `dist/linux`.

### Windows (Cross-compile on Linux)

1. Ensure `x86_64-w64-mingw32-g++` is installed and in your PATH.
2. Run the following command to build the Windows executable:
   ```bash
   make win
   ```
   This will create the Windows executable at `dist/win.exe`.

### Windows (Native Build)

- Use Visual Studio or MinGW on Windows to compile the source files manually.

---

## Run Instructions

### Linux

Run the compiled executable:

```bash
./dist/linux
```

### Windows

Run the compiled executable:

```bash
./dist/win.exe
```

---

## Testing

### Run All Tests

To build and run the full test suite:

```bash
make test
```

This compiles `tests/test_runner.cpp` and `src/utils/test_file_utils.cpp` into `dist/test_runner` and runs it.

### Run Utility Tests

To build and run the utility tests for file utilities:

```bash
make test-utils
```

### Run a Single Test File

To compile and run a single test file, use:

```bash
make test-f file=path/to/test_file.cpp
```

Replace `path/to/test_file.cpp` with the relative path to the test source file.

---

## Cleaning Build Artifacts

To remove all compiled binaries and the `dist` directory:

```bash
make clean
```

---

## Notes

- On Linux, ensure your system auto-mounts USB devices (GNOME/KDE usually handle this).
- On Windows, the app listens to system device events using `WM_DEVICECHANGE`.
- The project uses C++17 standard and links against platform-specific libraries (`libudev` on Linux, Windows API libraries on Windows).
- The test framework is a simple custom framework using macros to register and run tests.

---

## Project Structure

```
usb-monitor/
├── Makefile
├── README.md
├── DEVELOPER_DOC.md
├── include/
│   ├── platform/usb_monitor.h
│   └── utils/file_utils.h
├── src/
│   ├── main.cpp
│   ├── platform/usb_monitor_linux.cpp
│   ├── platform/usb_monitor_windows.cpp
│   └── utils/file_utils.cpp
├── tests/
│   └── test_runner.cpp
```

---

## License

MIT License – free for personal and commercial use.
