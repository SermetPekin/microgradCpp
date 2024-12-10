
#ifndef SIMPLE_TEST_H
#define SIMPLE_TEST_H
#include <iostream>
#include <string>
#include <vector>
#include <functional>
// Namespace to contain testing functionality
namespace simple_test
{
    // Struct to represent a single test case
    struct TestCase
    {
        std::string name;
        std::function<void()> func;
    };
    // Container for test cases
    std::vector<TestCase> test_cases;
// Macros to define and register test cases
#define TEST_CASE(name)                                                  \
    void name();                                                         \
    bool _##name##_registered = simple_test::register_test(#name, name); \
    void name()
    // Function to register a test case
    inline bool register_test(const std::string &name, std::function<void()> func)
    {
        test_cases.push_back({name, func});
        return true;
    }
#define EXPECT(condition)                                                                              \
    do                                                                                                 \
    {                                                                                                  \
        simple_test::tests_run++;                                                                      \
        if (!(condition))                                                                              \
        {                                                                                              \
            std::cerr << COLOR_RED << "[FAIL] " << COLOR_RESET                                         \
                      << __FILE__ << ":" << __LINE__ << " Tested [" << #condition << "]" << std::endl; \
            simple_test::tests_failed++;                                                               \
        }                                                                                              \
        else                                                                                           \
        {                                                                                              \
            std::cout << COLOR_GREEN << "[PASS] " << COLOR_RESET                                       \
                      << __FILE__ << ":" << __LINE__ << ": " << #condition << std::endl;               \
        }                                                                                              \
    } while (0)
#define OK(msg)                                 \
    do                                          \
    {                                           \
        simple_test::tests_run++;               \
        std::cout << "just passing ..." << msg; \
    } while (0)
// Assertion macros
#define EXPECT_TRUE(condition)                                                                                       \
    do                                                                                                               \
    {                                                                                                                \
        if (!(condition))                                                                                            \
        {                                                                                                            \
            std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << ": " << #condition << " is false" << std::endl; \
        }                                                                                                            \
        else                                                                                                         \
        {                                                                                                            \
            std::cout << "[PASS] " << __FILE__ << ":" << __LINE__ << ": " << #condition << std::endl;                \
        }                                                                                                            \
    } while (0)
#define EXPECT_EQ(actual, expected)                                                                                                    \
    do                                                                                                                                 \
    {                                                                                                                                  \
        if ((actual) != (expected))                                                                                                    \
        {                                                                                                                              \
            std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << ": Expected " << (expected) << ", got " << (actual) << std::endl; \
        }                                                                                                                              \
        else                                                                                                                           \
        {                                                                                                                              \
            std::cout << "[PASS] " << __FILE__ << ":" << __LINE__ << ": " << (actual) << " == " << (expected) << std::endl;            \
        }                                                                                                                              \
    } while (0)
#define EXPECT_NEAR(actual, expected, tolerance)                                \
    do                                                                          \
    {                                                                           \
        simple_test::tests_run++;                                               \
        if (std::fabs((actual) - (expected)) > (tolerance))                     \
        {                                                                       \
            std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__               \
                      << ": Expected " << (expected) << " but got " << (actual) \
                      << " (Tolerance: " << (tolerance) << ")" << std::endl;    \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            std::cout << "[PASS] " << __FILE__ << ":" << __LINE__               \
                      << ": " << (actual) << " is within " << (tolerance)       \
                      << " of " << (expected) << std::endl;                     \
        }                                                                       \
    } while (0)
    // Function to run all registered tests
    inline void run_all_tests()
    {
        int tests_run = 0;
        int tests_failed = 0;
        for (const auto &test : test_cases)
        {
            std::cout << "\nRunning test: " << test.name << std::endl;
            try
            {
                test.func();
            }
            catch (const std::exception &e)
            {
                std::cerr << "[EXCEPTION] " << e.what() << std::endl;
                tests_failed++;
            }
            tests_run++;
        }
        std::cout << "\nTests run: " << tests_run << std::endl;
        std::cout << "Tests failed: " << tests_failed << std::endl;
        if (tests_failed == 0)
        {
            std::cout << "All tests passed!" << std::endl;
        }
    }
} // namespace simple_test
#endif // SIMPLE_TEST_H