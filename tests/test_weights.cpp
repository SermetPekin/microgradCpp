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
#include <iostream>
#include <vector>
#include <memory>

class MLPTest : public ::testing::Test {
protected:
    // Setup code: This will be run before each test.
    MLP model = MLP(4, {7, 7, 3});  // Model with 4 input features, two hidden layers, 3 outputs
    SGD optimizer = SGD(0.01);     // Learning rate = 0.01
    std::vector<std::vector<std::shared_ptr<Value>>> inputs;
    std::vector<std::vector<std::shared_ptr<Value>>> targets;

    // Optional: Any data that you want to share across multiple tests
    void SetUp() override {
        // Sample input and target data (simplified)
        inputs = {
            {std::make_shared<Value>(1.0), std::make_shared<Value>(2.0), std::make_shared<Value>(3.0), std::make_shared<Value>(4.0)},
            {std::make_shared<Value>(4.0), std::make_shared<Value>(3.0), std::make_shared<Value>(2.0), std::make_shared<Value>(1.0)}
        };
        targets = {
            {std::make_shared<Value>(1.0), std::make_shared<Value>(0.0), std::make_shared<Value>(0.0)},
            {std::make_shared<Value>(0.0), std::make_shared<Value>(1.0), std::make_shared<Value>(0.0)}
        };
    }
};

TEST_F(MLPTest, WeightsUpdate) {
    auto initial_params = model.parameters();
    std::vector<double> initial_weights;
    for (const auto& param : initial_params) {
        initial_weights.push_back(param->data);
    }

    // Perform forward pass
    auto predictions = model.forward(inputs[0], true);

    // Compute loss (Cross-Entropy)
    auto loss = Loss::cross_entropy(predictions, targets[0]);

    // Perform backward pass
    optimizer.zero_grad(model.parameters());
    loss->backward();

    // Check the gradients before the optimizer step
    for (const auto& param : model.parameters()) {
        std::cout << "Grad before optimizer step: " << param->grad << std::endl;
    }

    // Update weights using the optimizer
    optimizer.step(model.parameters());

    // Check the gradients after the optimizer step
    for (const auto& param : model.parameters()) {
        std::cout << "Grad after optimizer step: " << param->grad << std::endl;
    }

    // Save updated weights
    auto updated_params = model.parameters();
    std::vector<double> updated_weights;
    for (const auto& param : updated_params) {
        updated_weights.push_back(param->data);
    }

    // Ensure that the weights have changed
    bool weights_changed = false;
    for (size_t i = 0; i < initial_weights.size(); ++i) {
        if (initial_weights[i] != updated_weights[i]) {
            weights_changed = true;
            break;
        }
    }

    // Assert that weights have changed after optimizer step
    ASSERT_TRUE(weights_changed) << "Weights did not change after optimizer step!";
}
 

