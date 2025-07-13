#include "utils/file_utils.h"
#include "utils/logger.h"
#include <filesystem>

namespace fs = std::filesystem;

void listFiles(const std::string &path)
{

    auto &logger = Logger::getInstance();
    logger.log(Logger::Level::Info, "Contents of " + path + ":");
    try
    {
        for (const auto &entry : fs::directory_iterator(path))
        {
            std::string msg = (entry.is_directory() ? "[DIR]  " : "       ") +
                              entry.path().filename().string();
            logger.log(Logger::Level::Info, msg);
        }
    }
    catch (...)
    {
        logger.log(Logger::Level::Error, "Failed to list files in " + path);
    }
}
