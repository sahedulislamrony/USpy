#ifdef _WIN32

#include "platform/usb_monitor.h"
#include "utils/file_utils.h"

#include <windows.h>
#include <dbt.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

void detectRemovableDrives() {
    DWORD drives = GetLogicalDrives();
    for (char drive = 'A'; drive <= 'Z'; ++drive) {
        if (drives & (1 << (drive - 'A'))) {
            std::string root = std::string(1, drive) + ":\\";
            UINT type = GetDriveTypeA(root.c_str());
            if (type == DRIVE_REMOVABLE) {
                std::cout << "\n📦 Removable drive found: " << root << std::endl;
                listFiles(root);
            }
        }
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_DEVICECHANGE) {
        if (wParam == DBT_DEVICEARRIVAL) {
            std::cout << "\n🔌 USB inserted!\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            detectRemovableDrives();
        } else if (wParam == DBT_DEVICEREMOVECOMPLETE) {
            std::cout << "\n❌ USB removed!\n";
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void startUSBMonitor() {
    const char* className = "USBMonitorClass";
    WNDCLASSA wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = className;

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(0, className, "USBMonitor", 0, 0, 0, 0, 0,
                                HWND_MESSAGE, nullptr, nullptr, nullptr);

    std::cout << "🟢 Listening for USB events (Windows)...\n";

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
#endif
