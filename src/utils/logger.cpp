#include "logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <random>
#include <chrono>
#include <thread>

Logger::Logger() : consoleOutputEnabled(false), randomLogRunning(false)
{
}

Logger::~Logger()
{
    stopRandomLogging();
    if (logFile.is_open())
    {
        logFile.close();
    }
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::setLogFile(const std::string &filename)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (logFile.is_open())
    {
        logFile.close();
    }
    logFile.open(filename, std::ios::app);
}

void Logger::enableConsoleOutput(bool enable)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (consoleOutputEnabled != enable)
    {
        consoleOutputEnabled = enable;
        if (!consoleOutputEnabled)
        {
            startRandomLogging();
        }
        else
        {
            stopRandomLogging();
        }
    }
}

std::string Logger::levelToString(Level level)
{
    switch (level)
    {
    case Level::Info:
        return "INFO";
    case Level::Error:
        return "ERROR";
    case Level::Debug:
        return "DEBUG";
    default:
        return "UNKNOWN";
    }
}

void Logger::log(Level level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mtx);

    // Get current time string
    auto t = std::time(nullptr);
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream timeStream;
    timeStream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    std::string logMessage = timeStream.str() + " [" + levelToString(level) + "] " + message + "\n";

    if (logFile.is_open())
    {
        logFile << logMessage;
        logFile.flush();
    }

    if (consoleOutputEnabled || level == Level::Error)
    {
        if (level == Level::Error)
        {
            std::cerr << logMessage;
        }
        else
        {
            std::cout << logMessage;
        }
    }
}

void Logger::startRandomLogging()
{
    if (randomLogRunning)
        return;

    randomLogRunning = true;
    randomLogThread = std::thread([this]()
                                  {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, randomMessages.size() - 1);

        while (randomLogRunning)
        {
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (!randomLogRunning)
                    break;
                std::string message = randomMessages[dist(gen)];
                log(Level::Info, message);
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));
        } });
}

void Logger::stopRandomLogging()
{
    if (!randomLogRunning)
        return;

    randomLogRunning = false;
    if (randomLogThread.joinable())
    {
        randomLogThread.join();
    }
}

const std::vector<std::string> Logger::randomMessages = {
    "Windows system is updating...",
    "Operating system is updating...",
    "System maintenance in progress...",
    "Security patches are being applied...",
    "Background system tasks running...",
    "System performance optimization underway...",
    "Checking for system updates...",
    "System diagnostics running...",
    "Updating system drivers...",
    "System cleanup in progress..."};
