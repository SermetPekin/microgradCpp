/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/

#include <gtest/gtest.h>
#include "micrograd.hpp" // Assuming Value class is defined in value.hpp
#include "types.hpp" // Assuming Value class is defined in value.hpp
#include <vector>
#include <memory>
#include <cmath>
using namespace microgradCpp ; 

// Alias for convenience
// using v_shared_Value = std::vector<std::shared_ptr<Value>>;
// using shared_Value = std::shared_ptr<Value>;

// Forward declaration of softmaxLocal function
// v_shared_Value softmaxLocal(const v_shared_Value &inputs);

// // Helper function to create a shared_ptr<Value>
shared_Value make_value(double data) {
    return std::make_shared<Value>(data);
}

// -------------------- TEST CASES --------------------

// Test softmax on a simple set of inputs
TEST(SoftmaxTest, SimpleInputs) {
    v_shared_Value inputs = {make_value(1.0), make_value(2.0), make_value(3.0)};
    auto outputs = softmaxLocal(inputs);

    // Compute expected softmax values manually
    double exp1 = std::exp(1.0);
    double exp2 = std::exp(2.0);
    double exp3 = std::exp(3.0);
    double sum_exp = exp1 + exp2 + exp3;

    EXPECT_NEAR(outputs[0]->data, exp1 / sum_exp, 1e-5);
    EXPECT_NEAR(outputs[1]->data, exp2 / sum_exp, 1e-5);
    EXPECT_NEAR(outputs[2]->data, exp3 / sum_exp, 1e-5);
}

// Test softmax with all equal inputs
TEST(SoftmaxTest, EqualInputs) {
    v_shared_Value inputs = {make_value(1.0), make_value(1.0), make_value(1.0)};
    auto outputs = softmaxLocal(inputs);

    // When all inputs are equal, softmax outputs should be uniform
    double expected = 1.0 / 3.0;

    EXPECT_NEAR(outputs[0]->data, expected, 1e-5);
    EXPECT_NEAR(outputs[1]->data, expected, 1e-5);
    EXPECT_NEAR(outputs[2]->data, expected, 1e-5);
}

// Test softmax with negative inputs
TEST(SoftmaxTest, NegativeInputs) {
    v_shared_Value inputs = {make_value(-1.0), make_value(-2.0), make_value(-3.0)};
    auto outputs = softmaxLocal(inputs);

    double exp1 = std::exp(-1.0);
    double exp2 = std::exp(-2.0);
    double exp3 = std::exp(-3.0);
    double sum_exp = exp1 + exp2 + exp3;

    EXPECT_NEAR(outputs[0]->data, exp1 / sum_exp, 1e-5);
    EXPECT_NEAR(outputs[1]->data, exp2 / sum_exp, 1e-5);
    EXPECT_NEAR(outputs[2]->data, exp3 / sum_exp, 1e-5);
}

// Test softmax with a single input
TEST(SoftmaxTest, SingleInput) {
    v_shared_Value inputs = {make_value(5.0)};
    auto outputs = softmaxLocal(inputs);

    EXPECT_NEAR(outputs[0]->data, 1.0, 1e-5);
}

//// Test softmax with large inputs to check for numerical stability
// TEST(SoftmaxTest, LargeInputs) {
//     v_shared_Value inputs = {make_value(1000.0), make_value(1001.0), make_value(1002.0)};
//     auto outputs = softmaxLocal(inputs);

//     double exp1 = std::exp(1000.0 - 1002.0);
//     double exp2 = std::exp(1001.0 - 1002.0);
//     double exp3 = std::exp(1002.0 - 1002.0);
//     double sum_exp = exp1 + exp2 + exp3;

//     EXPECT_NEAR(outputs[0]->data, exp1 / sum_exp, 1e-5);
//     EXPECT_NEAR(outputs[1]->data, exp2 / sum_exp, 1e-5);
//     EXPECT_NEAR(outputs[2]->data, exp3 / sum_exp, 1e-5);
// }

