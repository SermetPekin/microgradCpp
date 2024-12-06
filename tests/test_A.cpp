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
#include <cmath>
#include <iostream>
#include <gtest/gtest.h>
#include "micrograd.hpp"  // Assuming this file contains your Value class

// Helper function to compare floating-point numbers
bool approx(double x, double y, double tol = 1e-5) {
    return std::fabs(x - y) < tol;
}

// Test for arithmetic chain (addition, multiplication, and logarithm)
TEST(ValueTest, ArithmeticChain) {
    auto a = std::make_shared<Value>(2.0, "a");
    auto b = std::make_shared<Value>(3.0, "b");

    auto c = a + b;       // c = a + b
    auto d = c * b;       // d = c * b
    auto loss = d->log(); // loss = log(d)

    // Check forward values using ASSERT_NEAR for floating point comparisons
    ASSERT_NEAR(c->data, 5.0, 1e-5); // a + b = 5
    ASSERT_NEAR(d->data, 15.0, 1e-5); // c * b = 15
    ASSERT_NEAR(loss->data, 2.7080502, 1e-5); // log(15) ≈ 2.7080502

    // Backward:
    loss->backward();

    // Check gradients
    ASSERT_NEAR(a->grad, 0.2, 1e-3); // Gradient of a (expected)
    ASSERT_NEAR(b->grad, 0.5333333, 1e-3); // Gradient of b (expected)
}

// Test for exponential and logarithmic functions
TEST(ValueTest, ExpLog) {
    auto x = std::make_shared<Value>(2.0, "x");
    auto y = x->exp();    // y = exp(x)
    auto z = y->log();    // z = log(y)

    // Check forward values
    ASSERT_NEAR(y->data, 7.389056, 1e-5); // exp(2)
    ASSERT_NEAR(z->data, 2.0, 1e-5);      // log(exp(2)) = 2

    // Backward:
    z->backward();

    // Check gradients
    ASSERT_NEAR(x->grad, 1.0, 1e-3); // Gradient of x (expected)
}

// Test for complex expression (loss = ((a - b) * (a / b))^2)
TEST(ValueTest, ComplexExpression) {
    auto a = std::make_shared<Value>(4.0, "a");
    auto b = std::make_shared<Value>(2.0, "b");

    auto w = a - b;     // w = a - b
    auto u = a / b;     // u = a / b
    auto product = w * u; // product = w * u
    auto loss = product->pow(2.0); // loss = (w * u)^2

    // Check forward
    ASSERT_NEAR(loss->data, 16.0, 1e-5); // (2 * 2)^2 = 16

    loss->backward();

    // Check backward results
    ASSERT_NEAR(a->grad, 24.0, 1e-3); // Gradient of a (expected)
    ASSERT_NEAR(b->grad, -32.0, 1e-3); // Gradient of b (expected)
}

 