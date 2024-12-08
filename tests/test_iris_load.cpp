#include <gtest/gtest.h>

#include <cmath>
#include "micrograd.hpp"

// Type alias for the loader function
using LoaderFunc = std::function<DatasetType()>;

// Parameterized test function for Iris dataset loaders
void TestIrisLoader(LoaderFunc loader)
{
    // Call the provided loader function to get the dataset
    DatasetType dataset = loader();

    // Check if the dataset has the expected number of samples
    constexpr size_t expected_samples = 150;
    ASSERT_EQ(dataset.size(), expected_samples);

    // Check the structure of the first sample (features and targets)
    auto &first_sample = dataset[0];

    // The Iris dataset should have 4 feature inputs and 1 target output
    ASSERT_EQ(first_sample.first.size(), 4);  // Features
    ASSERT_EQ(first_sample.second.size(), 1); // Target

    // Check that the feature values are valid doubles (not NaN)
    for (const auto &feature : first_sample.first)
    {
        ASSERT_FALSE(std::isnan(feature->data));
    }

    // Check that the target is within the expected class range [0, 1, 2]
    double target_value = first_sample.second[0]->data;
    ASSERT_TRUE(target_value == 0.0 || target_value == 1.0 || target_value == 2.0);

    // Optionally, check a few more samples for consistency
    for (size_t i = 0; i < 10; ++i)
    {
        auto &sample = dataset[i];
        ASSERT_EQ(sample.first.size(), 4);  // 4 features
        ASSERT_EQ(sample.second.size(), 1); // 1 target

        for (const auto &feature : sample.first)
        {
            ASSERT_FALSE(std::isnan(feature->data));
        }

        double target = sample.second[0]->data;
        ASSERT_TRUE(target == 0.0 || target == 1.0 || target == 2.0);
    }
}

// Test case for get_iris
TEST(DataProcessorTest, GetIrisLoadsCorrectly)
{
    TestIrisLoader(get_iris);
}

// Test case for get_iris2
TEST(DataProcessorTest, GetIris2LoadsCorrectly)
{
    TestIrisLoader(get_iris2);
}


 
