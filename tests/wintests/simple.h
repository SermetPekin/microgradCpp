
// simple_test.h
#ifndef SIMPLE_TEST_H
#define SIMPLE_TEST_H
#include <iostream>
#include <string>
namespace simple_test
{
    int tests_run = 0;
    int tests_failed = 0;
// ANSI escape codes for colors
#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_CYAN "\033[36m"
// Macro for checking conditions
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
    // Summary function
    inline void test_summary()
    {
        std::cout << COLOR_CYAN << "\nTests run: " << tests_run << COLOR_RESET << std::endl;
        if (tests_failed > 0)
        {
            std::cerr << COLOR_RED << "Tests failed: " << tests_failed << COLOR_RESET << std::endl;
        }
        else
        {
            std::cout << COLOR_GREEN << "All tests passed!" << COLOR_RESET << std::endl;
        }
    }
// Macro for checking conditions
#define OK(msg)                                 \
    do                                          \
    {                                           \
        simple_test::tests_run++;               \
        std::cout << "just passing ..." << msg; \
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
}
#endif // SIMPLE_TEST_H