#include "utils/file_utils.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void listFiles(const std::string& path) {
    std::cout << "\n📁 Contents of " << path << ":\n";
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            std::cout << (entry.is_directory() ? "[DIR]  " : "       ")
                      << entry.path().filename().string() << "\n";
        }
    } catch (...) {
        std::cerr << "⚠️ Failed to read contents of " << path << "\n";
    }
}
