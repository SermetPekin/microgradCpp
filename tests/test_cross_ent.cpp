#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include "value.hpp"
#include "loss.hpp"
#include "micrograd.hpp"
#include "types.hpp"
using namespace microgradCpp;

inline std::shared_ptr<Value> make_value(double data)
{
    return std::make_shared<Value>(data);
}

TEST(LossTest, CrossEntropyLoss)
{
    std::vector<std::shared_ptr<Value>> predictions = {
        make_value(0.7), // Class 0
        make_value(0.2), // Class 1
        make_value(0.1)  // Class 2
    };

    std::vector<std::shared_ptr<Value>> targets = {
        make_value(0.0), // Class 0
        make_value(1.0), // Class 1 (correct class)
        make_value(0.0)  // Class 2
    };

    auto loss = Loss::cross_entropy(predictions, targets);

    double expected_loss = -std::log(0.2);

    ASSERT_NEAR(loss->data, expected_loss, 1e-4);
}
