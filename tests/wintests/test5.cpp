
#include "sptest.h"
using namespace sptest;
// Sample test case 1
TEST_CASE(test_addition51)
{
    EXPECT_EQ(28 + 2, 30);
    EXPECT_EQ(1 + 1, 2);
}
// Sample test case 2
TEST_CASE(test_boolean52)
{
    EXPECT_TRUE(3 > 2);
    EXPECT_TRUE(0 == 1); // This will fail
}