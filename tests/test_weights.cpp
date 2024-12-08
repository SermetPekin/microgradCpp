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
#include "adam.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <memory>

class MLPTest : public ::testing::Test
{
protected:
    // Setup code: This will be run before each test.
    MLP model = MLP(4, {7, 7, 3}); // Model with 4 input features, two hidden layers, 3 outputs
    SGD optimizer = SGD(0.01);     // Learning rate = 0.01
    std::vector<std::vector<std::shared_ptr<Value>>> inputs;
    std::vector<std::vector<std::shared_ptr<Value>>> targets;

    // double learning_rate = 0.01;
    int epochs = 100;

    // Initialize Adam optimizer
    // AdamOptimizer adam_optimizer(params,  0.01 );

    // Optional: Any data that you want to share across multiple tests
    void SetUp() override
    {
        // Sample input and target data (simplified)
        inputs = {
            {std::make_shared<Value>(1.0), std::make_shared<Value>(2.0), std::make_shared<Value>(3.0), std::make_shared<Value>(4.0)},
            {std::make_shared<Value>(4.0), std::make_shared<Value>(3.0), std::make_shared<Value>(2.0), std::make_shared<Value>(1.0)}};
        targets = {
            {std::make_shared<Value>(1.0), std::make_shared<Value>(0.0), std::make_shared<Value>(0.0)},
            {std::make_shared<Value>(0.0), std::make_shared<Value>(1.0), std::make_shared<Value>(0.0)}};
    }
};

TEST_F(MLPTest, WeightsUpdate)
{
    auto initial_params = model.parameters();
    std::vector<double> initial_weights;
    for (const auto &param : initial_params)
    {
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
    for (const auto &param : model.parameters())
    {
        std::cout << "Grad before optimizer step: " << param->grad << std::endl;
    }

    // Update weights using the optimizer
    optimizer.step(model.parameters());

    // Check the gradients after the optimizer step
    for (const auto &param : model.parameters())
    {
        std::cout << "Grad after optimizer step: " << param->grad << std::endl;
    }

    // Save updated weights
    auto updated_params = model.parameters();
    std::vector<double> updated_weights;
    for (const auto &param : updated_params)
    {
        updated_weights.push_back(param->data);
    }

    // Ensure that the weights have changed
    bool weights_changed = false;
    for (size_t i = 0; i < initial_weights.size(); ++i)
    {
        if (initial_weights[i] != updated_weights[i])
        {
            weights_changed = true;
            break;
        }
    }

    // Assert that weights have changed after optimizer step
    ASSERT_TRUE(weights_changed) << "Weights did not change after optimizer step!";
}

TEST_F(MLPTest, ForwardPassOutput)
{
    auto predictions = model.forward(inputs[0], true);
    ASSERT_EQ(predictions.size(), 3) << "Forward pass output size should match the number of output neurons (3).";

    for (const auto &pred : predictions)
    {
        ASSERT_NE(pred, nullptr) << "Prediction contains a null pointer.";
    }
}

TEST_F(MLPTest, CrossEntropyLoss)
{
    auto predictions = model.forward(inputs[0], true);
    auto loss = Loss::cross_entropy(predictions, targets[0]);

    ASSERT_TRUE(std::isfinite(loss->data)) << "Loss should be a finite value.";
    ASSERT_GE(loss->data, 0.0) << "Cross-Entropy Loss should be non-negative.";
}
TEST_F(MLPTest, ZeroGradients)
{
    optimizer.zero_grad(model.parameters());

    for (const auto &param : model.parameters())
    {
        ASSERT_EQ(param->grad, 0.0) << "Gradients should be zeroed out before backward pass.";
    }
}
TEST_F(MLPTest, GradientComputation)
{
    auto predictions = model.forward(inputs[0], true);
    auto loss = Loss::cross_entropy(predictions, targets[0]);

    // Perform backward pass
    optimizer.zero_grad(model.parameters());
    loss->backward();

    bool gradients_found = false;
    for (const auto &param : model.parameters())
    {
        if (param->grad != 0.0)
        {
            gradients_found = true;
            break;
        }
    }

    ASSERT_TRUE(gradients_found) << "Gradients should be non-zero after backward pass.";
}

TEST_F(MLPTest, ConsistentOutputInEvaluationMode)
{
    auto predictions1 = model.forward(inputs[0], false);
    auto predictions2 = model.forward(inputs[0], false);

    for (size_t i = 0; i < predictions1.size(); ++i)
    {
        ASSERT_NEAR(predictions1[i]->data, predictions2[i]->data, 1e-6) << "Outputs should be consistent in evaluation mode.";
    }
}

TEST_F(MLPTest, EmptyInputs)
{
    // ColRows empty_inputs;
    // EXPECT_THROW(model.forward(empty_inputs, true), std::runtime_error) << "Model should throw an error for empty inputs.";

    std::vector<std::shared_ptr<Value>> empty_input;
    EXPECT_THROW(model.forward(empty_input, true), std::runtime_error) << "Model should throw an error for empty inputs.";
}

TEST_F(MLPTest, SingleDataPoint)
{
    ColRows single_input = {{std::make_shared<Value>(1.0), std::make_shared<Value>(2.0), std::make_shared<Value>(3.0), std::make_shared<Value>(4.0)}};
    ColRows single_target = {{std::make_shared<Value>(1.0), std::make_shared<Value>(0.0), std::make_shared<Value>(0.0)}};

    auto predictions = model.forward(single_input[0], true);
    auto loss = Loss::cross_entropy(predictions, single_target[0]);

    ASSERT_TRUE(std::isfinite(loss->data)) << "Loss should be finite for a single data point.";
}

// TEST_F(MLPTest, TrainingLoopConvergence) {
//     double previous_loss = std::numeric_limits<double>::max();
//     for (int epoch = 0; epoch < 10; ++epoch) {
//         auto predictions = model.forward(inputs[0], true);
//         auto loss = Loss::cross_entropy(predictions, targets[0]);

//         optimizer.zero_grad(model.parameters());
//         loss->backward();
//         optimizer.step(model.parameters());

//         std::cout << "Epoch " << epoch + 1 << ": Loss = " << loss->data << std::endl;

//         ASSERT_LT(loss->data, previous_loss) << "Loss did not decrease in epoch " << epoch + 1;
//         previous_loss = loss->data;
//     }
// }

// TEST_F(MLPTest, TrainingLoopConvergence2) {
//     double previous_loss = std::numeric_limits<double>::max();
//     for (int epoch = 0; epoch < 10; ++epoch) {
//         auto predictions = model.forward(inputs[0], true);
//         auto loss = Loss::cross_entropy(predictions, targets[0]);

//         std::cout << "Epoch " << epoch + 1 << ": Loss = " << loss->data << std::endl;

//         optimizer.zero_grad(model.parameters());
//         loss->backward();
//         optimizer.step(model.parameters());

//         // Verify weight updates
//         for (const auto& param : model.parameters()) {
//             std::cout << "Weight: " << param->data << ", Grad: " << param->grad << std::endl;
//         }

//         ASSERT_LT(loss->data, previous_loss) << "Loss did not decrease in epoch " << epoch + 1;
//         previous_loss = loss->data;
//     }
// }

TEST_F(MLPTest, TrainingLoopConvergenceAdam)
{
    double previous_loss = std::numeric_limits<double>::max();

    std::vector<std::shared_ptr<Value>> params = model.parameters();

    AdamOptimizer adam_optimizer(model.parameters(), 0.01);

    for (int epoch = 0; epoch < 10; ++epoch)
    {
        auto predictions = model.forward(inputs[0], true);
        auto loss = Loss::cross_entropy(predictions, targets[0]);

        std::cout << "Epoch " << epoch + 1 << ": Loss = " << loss->data << std::endl;

        adam_optimizer.zero_grad( );
        loss->backward();
        // adam_optimizer.step(model.parameters());
        adam_optimizer.step( );

        // Verify weight updates
        for (const auto &param : model.parameters())
        {
            std::cout << "Weight: " << param->data << ", Grad: " << param->grad << std::endl;
        }

        ASSERT_LT(loss->data, previous_loss) << "Loss did not decrease in epoch " << epoch + 1;
        previous_loss = loss->data;
    }
}