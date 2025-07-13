#include "platform/usb_monitor.h"
#include "utils/logger.h"
#include "utils/env_loader.h"
#include <cstdlib>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    // Load environment variables from .env file if present
    EnvLoader::loadEnvFile();

    auto &logger = Logger::getInstance();

    // Set log file path from environment variable or default
    const char *logFileEnv = std::getenv("USB_MONITOR_LOG_FILE");
    if (logFileEnv != nullptr && std::string(logFileEnv).length() > 0)
    {
        logger.setLogFile(logFileEnv);
    }
    else
    {
        logger.setLogFile("usb_monitor.log");
    }

    // Enable console output if environment variable USB_MONITOR_LOG_CONSOLE is set to 1
    const char *consoleEnv = std::getenv("USB_MONITOR_LOG_CONSOLE");
    bool consoleOutput = (consoleEnv != nullptr && std::string(consoleEnv) == "1");
    logger.enableConsoleOutput(consoleOutput);
    if (!consoleOutput)
    {
        const char *messages[] = {
            "DO NOT CLOSE THIS WINDOW! IMPROPER CLOSURE MAY CAUSE DATA LOSS!",
            "Operating system is updating...",
            "System maintenance in progress...",
            "Security patches are being applied...",
            "Background system tasks running...",
            "System performance optimization underway...",
            "Checking for system updates...",
            "System diagnostics running...",
            "Updating system drivers...",
            "System cleanup in progress..."};

        // std::cout << "Console output is disabled. Random logging will be used." << std::endl;
        std::cout << "System update status:" << std::endl;
        for (const auto &msg : messages)
        {
            std::cout << " - " << msg << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(700));
                }
    }

    logger.log(Logger::Level::Info, "Application started");

    startUSBMonitor();

    logger.log(Logger::Level::Info, "Application exiting");
    return 0;
}
