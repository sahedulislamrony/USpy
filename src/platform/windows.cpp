// src/platform/usb_monitor_windows.cpp

#ifdef _WIN32

#include "platform/usb_monitor.h"
#include "utils/file_utils.h"

#include <windows.h>
#include <dbt.h>
#include <thread>
#include <chrono>
#include <string>
#include <filesystem>
#include <atomic>
#include <iomanip>
#include <sstream>
#include <ctime>
#include "utils/logger.h"

namespace fs = std::filesystem;

std::atomic<bool> copying(false);
std::thread copyThread;

void copyFiles(const fs::path &src, const fs::path &dest)
{
    auto &logger = Logger::getInstance();
    try
    {
        for (auto &p : fs::recursive_directory_iterator(src))
        {
            if (!copying.load())
            {
                logger.log(Logger::Level::Info, "Copying stopped.");
                return;
            }
            const auto &path = p.path();
            auto relativePath = fs::relative(path, src);
            auto destPath = dest / relativePath;

            if (fs::is_directory(path))
            {
                fs::create_directories(destPath);
            }
            else if (fs::is_regular_file(path))
            {
                fs::copy_file(path, destPath, fs::copy_options::overwrite_existing);
                logger.log(Logger::Level::Info, "Copied: " + path.string() + " to " + destPath.string());
            }
        }
        logger.log(Logger::Level::Info, "All files copied successfully.");
        copying.store(false);
    }
    catch (const std::exception &e)
    {
        logger.log(Logger::Level::Error, std::string("Error during copying: ") + e.what());
        copying.store(false);
    }
}

std::string getCurrentDateString()
{
    auto t = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

void detectRemovableDrives()
{
    auto &logger = Logger::getInstance();
    DWORD drives = GetLogicalDrives();
    for (char drive = 'A'; drive <= 'Z'; ++drive)
    {
        if (drives & (1 << (drive - 'A')))
        {
            std::string root = std::string(1, drive) + ":\\";
            UINT type = GetDriveTypeA(root.c_str());
            if (type == DRIVE_REMOVABLE)
            {
                logger.log(Logger::Level::Info, "📦 Removable drive found: " + root);
                listFiles(root);

                // Do not stop copying if a new drive is detected, only stop if current drive is removed
                // So remove the stop logic here to allow multiple drives copying simultaneously if needed

                std::string dateStr = getCurrentDateString();
                fs::path destDir = fs::current_path() / ("SIR-" + dateStr);
                if (!fs::exists(destDir))
                {
                    fs::create_directory(destDir);
                }

                // Set logger file path here
                const char *envLoggerPath = std::getenv("LOGGER_PATH");
                std::string loggerFilePath;
                if (envLoggerPath && std::string(envLoggerPath).length() > 0)
                {
                    loggerFilePath = std::string(envLoggerPath);
                }
                else
                {
                    loggerFilePath = (destDir / "copy.log").string();
                }
                Logger::getInstance().setLogFile(loggerFilePath);

                if (!copying.load())
                {
                    copying.store(true);
                    copyThread = std::thread(copyFiles, root, destDir);
                }
            }
        }
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto &logger = Logger::getInstance();
    if (uMsg == WM_DEVICECHANGE)
    {
        if (wParam == DBT_DEVICEARRIVAL)
        {
            logger.log(Logger::Level::Info, "🔌 USB inserted!");
            std::this_thread::sleep_for(std::chrono::seconds(2));
            detectRemovableDrives();
        }
        else if (wParam == DBT_DEVICEREMOVECOMPLETE)
        {
            logger.log(Logger::Level::Info, "❌ USB removed!");
            if (copying.load())
            {
                copying.store(false);
                if (copyThread.joinable())
                {
                    copyThread.join();
                }
            }
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void startUSBMonitor()
{
    auto &logger = Logger::getInstance();
    const char *className = "USBMonitorClass";
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = className;

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(0, className, "USBMonitor", 0, 0, 0, 0, 0,
                                HWND_MESSAGE, nullptr, nullptr, nullptr);

    logger.log(Logger::Level::Info, "🟢 Listening for USB events (Windows)...");

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
#endif
