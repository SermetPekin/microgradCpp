
#include "micrograd.hpp"

#include <set>
#include <iostream>
#include <gtest/gtest.h>
// #include "iris.hpp"
// #include "test_main.cpp"
// Test for loading Iris dataset and verifying unique classes
TEST(IrisTest, LoadAndVerifyClasses) {
    std::vector<std::vector<std::shared_ptr<Value>>> inputs;
    std::vector<std::vector<std::shared_ptr<Value>>> targets;

    std::string url = "./data/iris.csv";

    try {
        // Load Iris dataset
        IrisLoader::load_iris(url, inputs, targets);

        // Collect unique target classes
        std::set<std::vector<double>> unique_classes;

        for (const auto& target : targets) {
            std::vector<double> class_vector;
            for (const auto& val : target) {
                class_vector.push_back(val->data);
            }
            unique_classes.insert(class_vector);
        }

        // Check if there are three unique target classes (Iris dataset typically has 3 classes)
        ASSERT_EQ(unique_classes.size(), 3) << "There should be exactly 3 unique target classes";

        // Optionally, you can check for specific values in the unique classes
        for (const auto& cls : unique_classes) {
            ASSERT_FALSE(cls.empty()) << "One of the target classes is empty";
        }

        // You could also check specific class values, depending on the expected dataset
        // For example, check if a particular class is in the set (depends on your data)
        // Example:
        // std::vector<double> expected_class = {1.0, 0.0, 0.0};
        // ASSERT_TRUE(unique_classes.find(expected_class) != unique_classes.end()) << "Expected class not found";

    } catch (const std::exception& e) {
        FAIL() << "Error loading Iris dataset: " << e.what();
    }
}

// // Main function to run tests
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);  // Initialize Google Test
//     return RUN_ALL_TESTS();  // Run all tests
// }
