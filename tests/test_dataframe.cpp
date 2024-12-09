#include <gtest/gtest.h>
#include <fstream>
#include "micrograd.hpp"
#include "dataframe.hpp"
#include "sp_testing_utils.hpp"

#include <vector>
#include <algorithm>
using namespace microgradCpp;

using namespace sptest;

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

TEST(DataFrameTest, LoadCSVParts)
{
       DataFrame df;
    df.from_csv("./data/iris.csv", true, ',');
    // df.to_csv("a.csv") ;

    // df.normalize(  );
    df.encode_column("variety");
    df.print();
    df.shuffle();
    df.print();

    std::string temp_file = "temp_iris.csv";

    df.to_csv(temp_file);

   
    df.from_csv(temp_file);

    df.encode_column("variety");

    v_string cols_ = {"variety", "variety"};
    df.encode_column(cols_);
    df.encode_column(Range(4, 5));


//   DataFrame df;
//     df.from_csv("./data/iris.csv", true, ',');
//     // df.to_csv("a.csv") ; 
//     string col_ = "variety" ; 
//     // df.normalize(  );
//     df.encode_column(col_ );
//     df.print();
//     df.shuffle();
//     df.print();

 


//     df.encode_column(col_ );

//     v_string cols_ = {col_ , col_ };
//     df.encode_column(cols_);
//     df.encode_column(Range(4, 5));

//     df.shuffle();

//     // df(Range(5), Range(2)) ;  // .print();
//     df(Range(2), cols_).print();

    auto columns = df.get_column_names();
    EXPECT_EQ(columns.size(), 5);

    // EXPECT_THAT(columns, ::testing::Contains("sepal_length"));

    //sepal.length,sepal.width,petal.length,petal.width,variety

    // ASSERT_TRUE(sptest::contains(columns, "petal.width")) << "'petal.width' not found in columns";

    // EXPECT_EQ(columns[0], "petal.width");
    // EXPECT_EQ(columns[4], col_ );

    std::remove(temp_file.c_str());
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
