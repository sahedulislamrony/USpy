#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <thread>
#include <atomic>
#include <vector>

class Logger
{
public:
    enum class Level
    {
        Info,
        Error,
        Debug
    };

    static Logger &getInstance();

    void log(Level level, const std::string &message);
    void setLogFile(const std::string &filename);
    void enableConsoleOutput(bool enable);

private:
    Logger();
    ~Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    std::ofstream logFile;
    std::mutex mtx;
    bool consoleOutputEnabled;
    std::string levelToString(Level level);

    // New members for random logging when console output is disabled
    std::thread randomLogThread;
    std::atomic<bool> randomLogRunning;
    void startRandomLogging();
    void stopRandomLogging();
    static const std::vector<std::string> randomMessages;
};

#endif // LOGGER_H
