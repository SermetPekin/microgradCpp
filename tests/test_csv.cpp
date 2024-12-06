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
#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <stdexcept>

constexpr std::string iris_path= "./data/iris.csv"; 

// Test if the CSV file is loaded successfully
TEST(CSVLoaderTest, LoadCSVSuccess) {
    try {
        // Load the CSV file
        auto data = CSVLoader::load_csv(iris_path , true);

        // Check if the data is not empty
        ASSERT_FALSE(data.empty()) << "CSV data should not be empty.";

        // Check the number of rows (assuming Iris dataset has 150 rows)
        EXPECT_EQ(data.size(), 150) << "Unexpected number of rows in the dataset.";

        // Check the number of columns in the first row (assuming 5 columns: 4 features + label)
        EXPECT_EQ(data[0].size(), 5) << "Unexpected number of columns in the dataset.";

    } catch (const std::exception& e) {
        FAIL() << "Exception thrown during CSV loading: " << e.what();
    }
}
/*
// Test behavior with missing file
TEST(CSVLoaderTest, LoadCSVFileNotFound) {
    try {
        auto data = CSVLoader::load_csv("./data/non_existent.csv", true);
        FAIL() << "Expected std::runtime_error for missing file, but none was thrown.";
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Failed to open file: ./data/non_existent.csv");
    } catch (...) {
        FAIL() << "Expected std::runtime_error, but got a different exception.";
    }
}
*/

// Test loading CSV without header skipping
TEST(CSVLoaderTest, LoadCSVNoHeaderSkip) {
    try {
        auto data = CSVLoader::load_csv( iris_path , false);

        // Check if the data includes the header row
        ASSERT_FALSE(data.empty()) << "CSV data should not be empty.";
        EXPECT_EQ(data[0][0], "sepal.length") << "Expected header row as the first row.";
    } catch (const std::exception& e) {
        FAIL() << "Exception thrown during CSV loading: " << e.what();
    }
}

// Test loading CSV with specific row and column checks
TEST(CSVLoaderTest, LoadCSVContentValidation) {
    try {
        auto data = CSVLoader::load_csv( iris_path , true);

        // Validate specific cell content
        EXPECT_EQ(data[0][0], "5.1") << "Unexpected value in the first cell.";
        EXPECT_EQ(data[0][4], "Setosa") << "Unexpected value in the last cell of the first row.";
    } catch (const std::exception& e) {
        FAIL() << "Exception thrown during CSV loading: " << e.what();
    }
}

// // Main function for running tests
// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
