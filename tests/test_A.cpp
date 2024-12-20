/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#include <cmath>
#include <iostream>
#include <gtest/gtest.h>
#include "micrograd.hpp"   

 
bool approx(double x, double y, double tol = 1e-5) {
    return std::fabs(x - y) < tol;
}

 
TEST(ValueTest, ArithmeticChain) {
    auto a = std::make_shared<Value>(2.0, "a");
    auto b = std::make_shared<Value>(3.0, "b");

    auto c = a + b;       // c = a + b
    auto d = c * b;       // d = c * b
    auto loss = d->log(); // loss = log(d)

     
    ASSERT_NEAR(c->data, 5.0, 1e-5); // a + b = 5
    ASSERT_NEAR(d->data, 15.0, 1e-5); // c * b = 15
    ASSERT_NEAR(loss->data, 2.7080502, 1e-5); // log(15) ≈ 2.7080502

    // Backward:
    loss->backward();

    // Check gradients
    ASSERT_NEAR(a->grad, 0.2, 1e-3); // Gradient of a (expected)
    ASSERT_NEAR(b->grad, 0.5333333, 1e-3); // Gradient of b (expected)
}

 
TEST(ValueTest, ExpLog) {
    auto x = std::make_shared<Value>(2.0, "x");
    auto y = x->exp();    // y = exp(x)
    auto z = y->log();    // z = log(y)

 
    ASSERT_NEAR(y->data, 7.389056, 1e-5); // exp(2)
    ASSERT_NEAR(z->data, 2.0, 1e-5);      // log(exp(2)) = 2

  
    z->backward();

 
    ASSERT_NEAR(x->grad, 1.0, 1e-3); // Gradient of x (expected)
}

//   for complex expression (loss = ((a - b) * (a / b))^2)
TEST(ValueTest, ComplexExpression) {
    auto a = std::make_shared<Value>(4.0, "a");
    auto b = std::make_shared<Value>(2.0, "b");

    auto w = a - b;     // w = a - b
    auto u = a / b;     // u = a / b
    auto product = w * u; // product = w * u
    auto loss = product->pow(2.0); // loss = (w * u)^2

 
    ASSERT_NEAR(loss->data, 16.0, 1e-5); // (2 * 2)^2 = 16

    loss->backward();

 
    ASSERT_NEAR(a->grad, 24.0, 1e-3); // Gradient of a (expected)
    ASSERT_NEAR(b->grad, -32.0, 1e-3); // Gradient of b (expected)
}

 