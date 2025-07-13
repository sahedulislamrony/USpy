#ifndef _WIN32

#include "platform/usb_monitor.h"
#include "utils/file_utils.h"

#include <libudev.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

std::string getMountPoint(const std::string& devNode) {
    std::ifstream mounts("/proc/mounts");
    std::string line;
    while (std::getline(mounts, line)) {
        if (line.find(devNode) != std::string::npos) {
            std::istringstream iss(line);
            std::string device, mountPoint;
            iss >> device >> mountPoint;
            return mountPoint;
        }
    }
    return "";
}

void startUSBMonitor() {
    struct udev* udev = udev_new();
    if (!udev) {
        std::cerr << "❌ Cannot create udev\n";
        return;
    }

    struct udev_monitor* mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", "partition");
    udev_monitor_enable_receiving(mon);

    int fd = udev_monitor_get_fd(mon);

    std::cout << "🟢 Listening for USB events (Linux)...\n";

    while (true) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        if (select(fd + 1, &fds, nullptr, nullptr, nullptr) > 0) {
            struct udev_device* dev = udev_monitor_receive_device(mon);
            if (dev) {
                const char* action = udev_device_get_action(dev);
                const char* devNode = udev_device_get_devnode(dev);
                if (action && devNode) {
                    std::string devPath(devNode);
                    if (std::string(action) == "add") {
                        std::cout << "\n🔌 USB inserted: " << devPath << "\n";
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        std::string mountPath = getMountPoint(devPath);
                        if (!mountPath.empty()) {
                            std::cout << "📌 Mounted at: " << mountPath << "\n";
                            listFiles(mountPath);
                        } else {
                            std::cout << "⚠️ Not mounted.\n";
                        }
                    } else if (std::string(action) == "remove") {
                        std::cout << "\n❌ USB removed: " << devPath << "\n";
                    }
                }
                udev_device_unref(dev);
            }
        }
    }

    udev_unref(udev);
}
#endif
