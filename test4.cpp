
#include "sptest.h"
using namespace sptest;
// Sample test case 1
TEST_CASE(test_addition22)
{
    EXPECT_EQ(28 + 2, 30);
    EXPECT_EQ(1 + 1, 2);
}
// Sample test case 2
TEST_CASE(test_boolean22)
{
    EXPECT_TRUE(3 > 2);
    EXPECT_TRUE(0 == 1); // This will fail
}
using namespace sptest;
int main()
{
    sptest::run_all_tests();
    return 0;
}
/*
g++   test4.cpp test5.cpp -Iinclude -Iextern
*/