#include <gtest/gtest.h>

#include <cmath>
#include "micrograd.hpp"

// Type alias for the loader function
using LoaderFunc = std::function<DatasetType()>;

 
void TestIrisLoader(LoaderFunc loader)
{
     DatasetType dataset = loader();

     constexpr size_t expected_samples = 150;
    ASSERT_EQ(dataset.size(), expected_samples);

     auto &first_sample = dataset[0];

     ASSERT_EQ(first_sample.first.size(), 4);  // Features
    ASSERT_EQ(first_sample.second.size(), 1); // Target

     for (const auto &feature : first_sample.first)
    {
        ASSERT_FALSE(std::isnan(feature->data));
    }

      double target_value = first_sample.second[0]->data;
    ASSERT_TRUE(target_value == 0.0 || target_value == 1.0 || target_value == 2.0);

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

 
TEST(DataProcessorTest, GetIrisLoadsCorrectly)
{
    TestIrisLoader(get_iris);
}

 
TEST(DataProcessorTest, GetIris2LoadsCorrectly)
{
    TestIrisLoader(get_iris2);
}


 
