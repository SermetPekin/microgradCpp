#include <gtest/gtest.h>
#include <fstream>
#include "micrograd.hpp"
#include "dataframe.hpp"
#include "sp_testing_utils.hpp"

#include <vector>
#include <algorithm>
using namespace microgradCpp;

using namespace sptest ; 

 

// Helper function to create a temporary CSV file
// void create_temp_csv(const std::string &filename, const std::string &content)
// {
//     std::ofstream file(filename);
//     // ASSERT_TRUE(file.is_open());
//     file << content;
//     file.close();
// }

TEST(DataFrameTest, ValidateDataFrameContents)
{
    microgradCpp::DataFrame df;

    std::string temp_file = "temp_iris.csv";
    std::string csv_content =
        "sepal_length,sepal_width,petal_length,petal_width,species\n"
        "5.1,3.5,1.4,0.2,setosa\n"
        "7.0,3.2,4.7,1.4,versicolor\n"
        "6.3,3.3,6.0,2.5,virginica\n";

    create_temp_csv(temp_file, csv_content);
    df.from_csv(temp_file);

    auto sepal_length_values = df.values<double>("sepal_length");
    ASSERT_TRUE(sepal_length_values.has_value());
    EXPECT_EQ(sepal_length_values->size(), 3);
    EXPECT_DOUBLE_EQ(sepal_length_values->at(0), 5.1);
    EXPECT_DOUBLE_EQ(sepal_length_values->at(1), 7.0);
    EXPECT_DOUBLE_EQ(sepal_length_values->at(2), 6.3);

    auto species_values = df.values<std::string>("species");
    ASSERT_TRUE(species_values.has_value());
    EXPECT_EQ(species_values->at(0), "setosa");
    EXPECT_EQ(species_values->at(1), "versicolor");
    EXPECT_EQ(species_values->at(2), "virginica");

    std::remove(temp_file.c_str());






}

// Test encoding a categorical column in a DataFrame
TEST(DataFrameTest, EncodeCategoricalColumn)
{
    std::string temp_file = "temp_species.csv";
    std::string csv_content =
        "species\n"
        "setosa\n"
        "versicolor\n"
        "virginica\n"
        "setosa\n";

    create_temp_csv(temp_file, csv_content);

    microgradCpp::DataFrame df;
    df.from_csv(temp_file);

    df.encode_column("species");

    auto encoded_species = df.values<double>("species");
    ASSERT_TRUE(encoded_species.has_value());
    EXPECT_EQ(encoded_species->size(), 4);

    EXPECT_EQ(encoded_species->at(0), 0);
    EXPECT_EQ(encoded_species->at(1), 1);
    EXPECT_EQ(encoded_species->at(2), 2);
    EXPECT_EQ(encoded_species->at(3), 0);

    std::remove(temp_file.c_str());
}

// Test loading a CSV and checking inferred column types
TEST(DataFrameTest, CheckInferredTypes)
{
    std::string temp_file = "temp_mixed.csv";
    std::string csv_content =
        "col1,col2,col3\n"
        "42,3.14,hello\n"
        "100,2.71,world\n";

    create_temp_csv(temp_file, csv_content);

    microgradCpp::DataFrame df;
    df.from_csv(temp_file);

    EXPECT_EQ(df.get_column_type("col1"), typeid(double));
    EXPECT_EQ(df.get_column_type("col2"), typeid(double));
    EXPECT_EQ(df.get_column_type("col3"), typeid(std::string));

    std::remove(temp_file.c_str());
}

// Test to check loading and saving an Iris-like dataset
// TEST(DataFrameTest, LoadAndSaveCSV) {
//     std::string temp_file = "temp_iris.csv";
//     std::string csv_content =
//         "sepal_length,sepal_width,petal_length,petal_width,species\n"
//         "5.1,3.5,1.4,0.2,setosa\n"
//         "7.0,3.2,4.7,1.4,versicolor\n"
//         "6.3,3.3,6.0,2.5,virginica\n";

//     create_temp_csv(temp_file, csv_content);

//     microgradCpp::DataFrame df;
//     df.from_csv(temp_file);

//     auto columns = df.get_column_names();
//     EXPECT_EQ(columns.size(), 5);
//     EXPECT_EQ(columns[0], "sepal_length");
//     EXPECT_EQ(columns[4], "species");

//     auto sepal_length_values = df.values<double>("sepal_length");
//     ASSERT_TRUE(sepal_length_values.has_value());
//     EXPECT_EQ(sepal_length_values->size(), 3);
//     EXPECT_DOUBLE_EQ(sepal_length_values->at(0), 5.1);
//     EXPECT_DOUBLE_EQ(sepal_length_values->at(1), 7.0);
//     EXPECT_DOUBLE_EQ(sepal_length_values->at(2), 6.3);

//     auto species_values = df.values<std::string>("species");
//     ASSERT_TRUE(species_values.has_value());
//     EXPECT_EQ(species_values->at(0), "setosa");
//     EXPECT_EQ(species_values->at(1), "versicolor");
//     EXPECT_EQ(species_values->at(2), "virginica");

//     std::string output_file = "output_iris.csv";
//     df.to_csv(output_file);

//     std::ifstream infile(output_file);
//     ASSERT_TRUE(infile.is_open());

//     std::string line;
//     std::getline(infile, line);
//     EXPECT_EQ(line, "sepal_length;sepal_width;petal_length;petal_width;species");

//     std::getline(infile, line);
//     EXPECT_EQ(line, "5.1;3.5;1.4;0.2;setosa");

//     std::getline(infile, line);
//     EXPECT_EQ(line, "7.0;3.2;4.7;1.4;versicolor");

//     std::getline(infile, line);
//     EXPECT_EQ(line, "6.3;3.3;6.0;2.5;virginica");

//     infile.close();

//     std::remove(temp_file.c_str());
//     std::remove(output_file.c_str());
// }

// TEST(DataFrameTest, LoadCSV) {
//     std::string temp_file = "temp_iris.csv";
//     std::string csv_content =
//         "sepal_length,sepal_width,petal_length,petal_width,species\n"
//         "5.1,3.5,1.4,0.2,setosa\n"
//         "7.0,3.2,4.7,1.4,versicolor\n"
//         "6.3,3.3,6.0,2.5,virginica\n";

//     create_temp_csv(temp_file, csv_content);

//     microgradCpp::DataFrame df;
//     df.from_csv(temp_file);

//     auto columns = df.get_column_names();
//     EXPECT_EQ(columns.size(), 5);
//     EXPECT_EQ(columns[0], "sepal_length");
//     EXPECT_EQ(columns[4], "species");

//     std::remove(temp_file.c_str());
// }

TEST(DataFrameTest, LoadCSVParts)
{
    std::string temp_file = "temp_iris.csv";
    std::string csv_content =
        "sepal_length,sepal_width,petal_length,petal_width,species\n"
        "5.1,3.5,1.4,0.2,setosa\n"
        "7.0,3.2,4.7,1.4,versicolor\n"
        "6.3,3.3,6.0,2.5,virginica\n";

    create_temp_csv(temp_file, csv_content);

    microgradCpp::DataFrame df;
    df.from_csv(temp_file);

    auto columns = df.get_column_names();
    EXPECT_EQ(columns.size(), 5);

    // EXPECT_THAT(columns, ::testing::Contains("sepal_length"));

    ASSERT_TRUE(sptest::contains(columns, "sepal_length")) << "'sepal_length' not found in columns";

    EXPECT_EQ(columns[0], "sepal_length");
    EXPECT_EQ(columns[4], "species");

    // std::remove(temp_file.c_str());
}

// TEST(DataFrameTest, SaveCSV) {
//     microgradCpp::DataFrame df;

//     std::string temp_file = "temp_iris.csv";
//     std::string csv_content =
//         "sepal_length,sepal_width,petal_length,petal_width,species\n"
//         "5.1,3.5,1.4,0.2,setosa\n"
//         "7.0,3.2,4.7,1.4,versicolor\n"
//         "6.3,3.3,6.0,2.5,virginica\n";

//     create_temp_csv(temp_file, csv_content);
//     df.from_csv(temp_file);

//     std::string output_file = "output_iris.csv";
//     df.to_csv(output_file);

//     std::ifstream infile(output_file);
//     ASSERT_TRUE(infile.is_open());

//     std::string line;
//     std::getline(infile, line);
//     EXPECT_EQ(line, "sepal_length;sepal_width;petal_length;petal_width;species");

//     std::getline(infile, line);
//     EXPECT_EQ(line, "5.1;3.5;1.4;0.2;setosa");

//     std::getline(infile, line);
//     EXPECT_EQ(line, "7.0;3.2;4.7;1.4;versicolor");

//     std::getline(infile, line);
//     EXPECT_EQ(line, "6.3;3.3;6.0;2.5;virginica");

//     infile.close();

//     std::remove(temp_file.c_str());
//     std::remove(output_file.c_str());
// }
