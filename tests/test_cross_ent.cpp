#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include "value.hpp"
#include "loss.hpp"
#include "micrograd.hpp"
#include "types.hpp"
using namespace microgradCpp;

// Helper function to create a Value pointer
std::shared_ptr<Value> make_value(double data)
{
    return std::make_shared<Value>(data);
}

// Test for Loss::cross_entropy
TEST(LossTest, CrossEntropyLoss)
{
    // Example predictions: probabilities for 3 classes
    std::vector<std::shared_ptr<Value>> predictions = {
        make_value(0.7), // Class 0
        make_value(0.2), // Class 1
        make_value(0.1)  // Class 2
    };

    // Target: class 1 (one-hot encoded)
    std::vector<std::shared_ptr<Value>> targets = {
        make_value(0.0), // Class 0
        make_value(1.0), // Class 1 (correct class)
        make_value(0.0)  // Class 2
    };

    // Compute the cross-entropy loss
    auto loss = Loss::cross_entropy(predictions, targets);

    // Expected cross-entropy loss: -log(p_1) = -log(0.2) ≈ 1.6094
    double expected_loss = -std::log(0.2);

    // Check if the computed loss is approximately equal to the expected loss
    ASSERT_NEAR(loss->data, expected_loss, 1e-4);
}
