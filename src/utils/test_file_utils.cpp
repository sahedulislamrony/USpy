#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "../tests/test_runner.cpp"

namespace fs = std::filesystem;

bool copyFilesForTest(const fs::path &src, const fs::path &dest)
{
    try
    {
        for (auto &p : fs::recursive_directory_iterator(src))
        {
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
            }
        }
        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::string getCurrentDateStringForTest()
{
    auto t = std::time(nullptr);
    std::tm tm;
    localtime_r(&t, &tm);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

REGISTER_TEST(testCopyFiles)
{
    std::cout << "Running testCopyFiles...\n";

    fs::path testSrc = "test_src";
    fs::path testDest = "test_dest";

    // Clean up any previous test folders
    fs::remove_all(testSrc);
    fs::remove_all(testDest);

    // Create test source folder and files
    fs::create_directory(testSrc);
    std::ofstream(testSrc / "file1.txt") << "Hello World";
    fs::create_directory(testSrc / "subdir");
    std::ofstream(testSrc / "subdir/file2.txt") << "Test file 2";

    // Run copy
    bool result = copyFilesForTest(testSrc, testDest);
    TEST_ASSERT(result);

    // Check files copied
    TEST_ASSERT(fs::exists(testDest / "file1.txt"));
    TEST_ASSERT(fs::exists(testDest / "subdir/file2.txt"));

    // Read and verify content
    std::ifstream f1(testDest / "file1.txt");
    std::string content1((std::istreambuf_iterator<char>(f1)), std::istreambuf_iterator<char>());
    TEST_ASSERT(content1 == "Hello World");

    std::ifstream f2(testDest / "subdir/file2.txt");
    std::string content2((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());
    TEST_ASSERT(content2 == "Test file 2");

    // Cleanup
    fs::remove_all(testSrc);
    fs::remove_all(testDest);

    std::cout << "testCopyFiles passed.\n";
    return true;
}

REGISTER_TEST(testGetCurrentDateString)
{
    std::cout << "Running testGetCurrentDateString...\n";
    std::string dateStr = getCurrentDateStringForTest();
    std::cout << "Date string: " << dateStr << "\n";
    // Basic check for format YYYY-MM-DD length 10
    TEST_ASSERT(dateStr.size() == 10);
    TEST_ASSERT(dateStr[4] == '-');
    TEST_ASSERT(dateStr[7] == '-');
    std::cout << "testGetCurrentDateString passed.\n";
    return true;
}
