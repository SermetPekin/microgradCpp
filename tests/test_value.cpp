/*
MIT License

Copyright (c) [2024] Sermet Pekin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "micrograd.hpp"
#include <gtest/gtest.h>
#include <memory>

// Test multiplication of two Value objects
TEST(ValueTest, Multiply) {
    // Create two values
    auto x = std::make_shared<Value>(2.0, "x");
    auto y = std::make_shared<Value>(3.0, "y");

    // Perform multiplication
    auto z = x * y;  // z = x * y

    // Check the result of the multiplication
    ASSERT_EQ(z->data, 6.0);  // Expect z = 6.0
}

// Test addition of a Value object
TEST(ValueTest, Add) {
    // Create two values
    auto x = std::make_shared<Value>(2.0, "x");
    auto y = std::make_shared<Value>(3.0, "y");

    // Perform multiplication and then addition
    auto z = x * y;  // z = x * y
    auto w = z + x;  // w = z + x

    // Check the result of the addition
    ASSERT_EQ(w->data, 8.0);  // Expect w = 8.0
}

// Test backpropagation
TEST(ValueTest, Backward) {
    // Create two values
    auto x = std::make_shared<Value>(2.0, "x");
    auto y = std::make_shared<Value>(3.0, "y");

    // Perform multiplication and addition
    auto z = x * y;  // z = x * y
    auto w = z + x;  // w = z + x

    // Backpropagation
    w->backward();

    // Check gradients
    ASSERT_NEAR(x->grad, 4.0, 1e-3);  // Gradient of x
    ASSERT_NEAR(y->grad, 2.0, 1e-3);  // Gradient of y
}

// Main function for running the tests
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);  // Initialize Google Test
//     return RUN_ALL_TESTS();  // Run all tests
// }
