# 🔌 USB Monitor - Cross Platform (C++)

A lightweight C++ console application that monitors USB insertions/removals and lists file contents on both **Windows 11** and **Linux** systems.

---

## ✨ Features

- Detects USB device insertion/removal in real time
- Automatically identifies mount point or drive letter
- Lists all files and folders on the inserted USB device
- Supports both **Linux (via libudev)** and **Windows (via Windows API)**
- Clean, modular structure with separate platform and utility files

---

## 📁 Project Structure

```
usb-monitor/
├── Makefile
├── README.md
├── include/
│   ├── utils/file_utils.h
│   └── platform/usb_monitor.h
├── src/
│   ├── main.cpp
│   ├── utils/file_utils.cpp
│   ├── platform/usb_monitor_linux.cpp
│   └── platform/usb_monitor_windows.cpp
```

---

## 🛠️ Build Instructions

### ✅ Linux

1. **Install Dependencies**:
   ```bash
   sudo apt update
   sudo apt install g++ libudev-dev
   ```

2. **Build the App**:
   ```bash
   make
   ```

3. **Run**:
   ```bash
   ./usb_monitor
   ```

---

### ✅ Windows (with MinGW)

1. **Install MinGW** and add it to your PATH  
   Or use Visual Studio (manual compile).

2. **Build the App**:
   ```bash
   mingw32-make
   ```

3. **Run**:
   ```bash
   usb_monitor.exe
   ```

---

## 🧠 Notes

- On **Linux**, ensure your system auto-mounts USB devices (GNOME/KDE usually handle this).
- On **Windows**, the app listens to system device events using `WM_DEVICECHANGE`.

---

## 📌 Dependencies

- C++17+
- Linux: `libudev-dev`
- Windows: Windows.h, user32, gdi32

---

## 📃 License

MIT License – free for personal and commercial use.
