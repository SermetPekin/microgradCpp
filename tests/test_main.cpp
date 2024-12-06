#include <gtest/gtest.h>

// Main entry for Google Test framework
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
