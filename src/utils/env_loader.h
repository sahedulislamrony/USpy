#ifndef ENV_LOADER_H
#define ENV_LOADER_H

#include <string>

namespace EnvLoader
{
    // Loads environment variables from a .env file at the given path.
    // Returns true if the file was loaded successfully, false otherwise.
    bool loadEnvFile(const std::string &filepath = ".env");
}

#endif
