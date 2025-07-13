
#ifndef _WIN32

#include "platform/usb_monitor.h"
#include "utils/file_utils.h"

#include <libudev.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include "utils/logger.h"

std::string getMountPoint(const std::string &devNode)
{
    std::ifstream mounts("/proc/mounts");
    std::string line;
    while (std::getline(mounts, line))
    {
        if (line.find(devNode) != std::string::npos)
        {
            std::istringstream iss(line);
            std::string device, mountPoint;
            iss >> device >> mountPoint;
            return mountPoint;
        }
    }
    return "";
}

#include <filesystem>
#include <thread>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>

namespace fs = std::filesystem;

std::atomic<bool> copying(false);
std::thread copyThread;
std::atomic<bool> copyThreadRunning(false);

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
                copyThreadRunning.store(false);
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
        copyThreadRunning.store(false);
    }
    catch (const std::exception &e)
    {
        logger.log(Logger::Level::Error, std::string("Error during copying: ") + e.what());
        copying.store(false);
        copyThreadRunning.store(false);
    }
}

std::string getCurrentDateString()
{
    auto t = std::time(nullptr);
    std::tm tm;
    localtime_r(&t, &tm);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    return oss.str();
}

#include <iostream>
#include <string>
#include <mutex>

std::string lastMountPath;
std::mutex lastMountPathMutex;

void restartCopying()
{
    auto &logger = Logger::getInstance();
    std::lock_guard<std::mutex> lock(lastMountPathMutex);
    if (lastMountPath.empty())
    {
        logger.log(Logger::Level::Info, "No USB device to restart copying.");
        return;
    }

    if (copying.load())
    {
        copying.store(false);
        if (copyThread.joinable() && copyThreadRunning.load())
        {
            try
            {
                copyThread.join();
            }
            catch (const std::exception &e)
            {
                logger.log(Logger::Level::Error, std::string("Exception joining copyThread: ") + e.what());
            }
        }
    }

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

    copying.store(true);
    copyThreadRunning.store(true);
    copyThread = std::thread(copyFiles, lastMountPath, destDir);
    logger.log(Logger::Level::Info, "Restarted copying for USB mounted at: " + lastMountPath);
}

void startUSBMonitor()
{
    auto &logger = Logger::getInstance();

    struct udev *udev = udev_new();
    if (!udev)
    {
        logger.log(Logger::Level::Error, "❌ Cannot create udev");
        return;
    }

    struct udev_monitor *mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", "partition");
    udev_monitor_enable_receiving(mon);

    int fd = udev_monitor_get_fd(mon);

    logger.log(Logger::Level::Info, "🟢 Listening for USB events (Linux)...");

    while (true)
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        FD_SET(STDIN_FILENO, &fds);

        int maxfd = std::max(fd, STDIN_FILENO);

        if (select(maxfd + 1, &fds, nullptr, nullptr, nullptr) > 0)
        {
            if (FD_ISSET(fd, &fds))
            {
                struct udev_device *dev = udev_monitor_receive_device(mon);
                if (dev)
                {
                    const char *action = udev_device_get_action(dev);
                    const char *devNode = udev_device_get_devnode(dev);
                    if (action && devNode)
                    {
                        std::string devPath(devNode);
                        if (std::string(action) == "add")
                        {
                            logger.log(Logger::Level::Info, "🔌 USB inserted: " + devPath);
                            std::this_thread::sleep_for(std::chrono::seconds(2));
                            std::string mountPath = getMountPoint(devPath);
                            if (!mountPath.empty())
                            {
                                logger.log(Logger::Level::Info, "📌 Mounted at: " + mountPath);
                                listFiles(mountPath);

                                {
                                    std::lock_guard<std::mutex> lock(lastMountPathMutex);
                                    lastMountPath = mountPath;
                                }

                                if (copying.load())
                                {
                                    copying.store(false);
                                    if (copyThread.joinable() && copyThreadRunning.load())
                                    {
                                        try
                                        {
                                            copyThread.join();
                                        }
                                        catch (const std::exception &e)
                                        {
                                            logger.log(Logger::Level::Error, std::string("Exception joining copyThread: ") + e.what());
                                        }
                                    }
                                }

                                std::string dateStr = getCurrentDateString();
                                fs::path destDir = fs::current_path() / ("SIR-" + dateStr);
                                if (!fs::exists(destDir))
                                {
                                    fs::create_directory(destDir);
                                }

                                copying.store(true);
                                copyThreadRunning.store(true);
                                copyThread = std::thread(copyFiles, mountPath, destDir);

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
                            }
                            else
                            {
                                logger.log(Logger::Level::Info, "⚠️ Not mounted.");
                            }
                        }
                        else if (std::string(action) == "remove")
                        {
                            logger.log(Logger::Level::Info, "❌ USB removed: " + devPath);
                            if (copying.load())
                            {
                                copying.store(false);
                                if (copyThread.joinable() && copyThreadRunning.load())
                                {
                                    try
                                    {
                                        copyThread.join();
                                    }
                                    catch (const std::exception &e)
                                    {
                                        logger.log(Logger::Level::Error, std::string("Exception joining copyThread: ") + e.what());
                                    }
                                }
                            }
                        }
                    }
                    udev_device_unref(dev);
                }
            }
            if (FD_ISSET(STDIN_FILENO, &fds))
            {
                std::string input;
                std::getline(std::cin, input);
                if (input == "rs")
                {
                    restartCopying();
                }
            }
        }
    }

    udev_unref(udev);
}
#endif
