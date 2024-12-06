#include "micrograd.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>

// Test adding and retrieving numeric columns
TEST(TableTest, AddAndRetrieveNumericColumn) {
    Table table;

    // Add numeric columns
    table.add_column("Feature1", std::vector<double>{1.0, 2.0, 3.0});
    table.add_column("Feature2", std::vector<double>{4.0, 5.0, 6.0});

    // Retrieve and verify columns
    auto feature1 = table.get_column("Feature1");
    ASSERT_EQ(feature1->name, "Feature1");
    ASSERT_EQ(feature1->size(), 3);
    ASSERT_EQ(std::get<std::vector<double>>(feature1->data)[0], 1.0);
    ASSERT_EQ(std::get<std::vector<double>>(feature1->data)[2], 3.0);

    auto feature2 = table.get_column("Feature2");
    ASSERT_EQ(feature2->name, "Feature2");
    ASSERT_EQ(feature2->size(), 3);
    ASSERT_EQ(std::get<std::vector<double>>(feature2->data)[1], 5.0);
}

// Test adding and retrieving a categorical column
TEST(TableTest, AddAndRetrieveCategoricalColumn) {
    Table table;

    // Add a categorical column
    table.add_column("Label", std::vector<std::string>{"Setosa", "Versicolor", "Virginica"});

    // Retrieve and verify the column
    auto label = table.get_column("Label");
    ASSERT_EQ(label->name, "Label");
    ASSERT_EQ(label->size(), 3);
    ASSERT_EQ(std::get<std::vector<std::string>>(label->data)[0], "Setosa");
    ASSERT_EQ(std::get<std::vector<std::string>>(label->data)[2], "Virginica");
}

// Test getting a specific row
TEST(TableTest, GetRow) {
    Table table;

    // Add numeric and categorical columns
    table.add_column("Feature1", std::vector<double>{1.0, 2.0, 3.0});
    table.add_column("Feature2", std::vector<double>{4.0, 5.0, 6.0});
    table.add_column("Label", std::vector<std::string>{"Setosa", "Versicolor", "Virginica"});

    // Retrieve and verify row 1
    auto row = table.get_row(1);
    ASSERT_EQ(row.size(), 3); // Row should have data from all three columns
    ASSERT_EQ(row[0], "2.000000");
    ASSERT_EQ(row[1], "5.000000");
    ASSERT_EQ(row[2], "Versicolor");
}

// Test printing a column
TEST(TableTest, PrintColumn) {
    Table table;

    // Add a numeric column
    table.add_column("Feature1", std::vector<double>{1.0, 2.0, 3.0});

    // Print the column (to verify manually or integrate better validation)
    auto feature1 = table.get_column("Feature1");
    ASSERT_NO_THROW(feature1->print());
}

// Test edge case: Accessing non-existent column
TEST(TableTest, AccessNonExistentColumn) {
    Table table;

    // Try to access a column that doesn't exist
    ASSERT_THROW(table.get_column("NonExistent"), std::runtime_error);
}

// Test edge case: Getting a row out of bounds
TEST(TableTest, GetRowOutOfBounds) {
    Table table;

    // Add columns with fewer rows
    table.add_column("Feature1", std::vector<double>{1.0});
    table.add_column("Label", std::vector<std::string>{"Setosa"});

    // Try to access an out-of-bounds row
    ASSERT_THROW(table.get_row(1), std::runtime_error);
}

// // Main function for running the tests
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
