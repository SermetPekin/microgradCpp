
#include "tests/wintests/simple2.h"
using namespace simple_test;
// Sample test case 1
TEST_CASE(test_addition22)
{
    EXPECT_EQ(2 + 2, 4);
    EXPECT_EQ(1 + 1, 2);
}
// Sample test case 2
TEST_CASE(test_boolean22)
{
    EXPECT_TRUE(3 > 2);
    EXPECT_TRUE(0 == 1); // This will fail
}