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
