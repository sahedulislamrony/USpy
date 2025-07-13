#include <iostream>
#include <vector>
#include <string>
#include <functional>

struct Test
{
    std::string name;
    std::function<bool()> func;
};

std::vector<Test> &getTests()
{
    static std::vector<Test> tests;
    return tests;
}

#define REGISTER_TEST(name)                                       \
    bool name();                                                  \
    struct Register##name                                         \
    {                                                             \
        Register##name() { getTests().push_back({#name, name}); } \
    } register##name;                                             \
    bool name()

#define TEST_ASSERT(cond)                                         \
    do                                                            \
    {                                                             \
        if (!(cond))                                              \
        {                                                         \
            std::cerr << "Assertion failed: " #cond << std::endl; \
            return false;                                         \
        }                                                         \
    } while (0)

int main()
{
    int passed = 0;
    int failed = 0;
    auto &tests = getTests();
    std::cout << "Running " << tests.size() << " tests:\n\n";
    for (auto &test : tests)
    {
        std::cout << "Running test: " << test.name << " ... ";
        bool result = test.func();
        if (result)
        {
            std::cout << "✅ OK\n";
            passed++;
        }
        else
        {
            std::cout << "❌ FAIL\n";
            failed++;
        }
    }
    std::cout << "\nTest summary: " << passed << " passed, " << failed << " failed.\n";
    return failed == 0 ? 0 : 1;
}
