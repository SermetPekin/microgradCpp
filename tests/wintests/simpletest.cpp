
#include "simple.h"
void test_addition()
{
    EXPECT(2 + 2 == 4);
    EXPECT(1 + 1 == 3); // This will fail
}
void test_subtraction()
{
    EXPECT(5 - 3 == 2);
    EXPECT(10 - 5 == 5);
}
int main()
{
    std::cout << "Running tests...\n"
              << std::endl;
    test_addition();
    test_subtraction();
    simple_test::test_summary();
    return 0;
}