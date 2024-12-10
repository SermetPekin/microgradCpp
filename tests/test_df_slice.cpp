/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#include <gtest/gtest.h>
#include <fstream>
#include "micrograd.hpp"
#include "dataframe.hpp"
#include "sp_testing_utils.hpp"

#include <gtest/gtest.h>

#include <vector>
#include <algorithm>
using namespace microgradCpp;

using namespace sptest;

 
DataFrame create_sample_dataframe()
{
    // DataFrame df;
    // df.add_column("sepal_length", {5.1, 7.0, 6.3, 4.9, 5.8}, typeid(double));
    // df.add_column("species", {"setosa", "versicolor", "virginica", "setosa", "versicolor"}, typeid(std::string));
    // return df;

    std::string temp_file = "temp_iris.csv";
    std::string csv_content =
        "sepal_length,sepal_width,petal_length,petal_width,species\n"
        "5.1,3.5,1.4,0.2,setosa\n"
        "7.0,3.2,4.7,1.4,versicolor\n"
        "6.3,3.3,6.0,2.5,virginica\n";

    create_temp_csv(temp_file, csv_content);
    microgradCpp::DataFrame df;
    df.from_csv(temp_file);
    std::remove(temp_file.c_str());
    return df;
}

 
TEST(DataFrameTest, ValidSliceWithInitializerList)
{
    DataFrame df = create_sample_dataframe();
    EXPECT_NO_THROW({
        df({0, 2}, {"species"}).print();
    });
}

 
TEST(DataFrameTest, OutOfBoundsSliceWithInitializerList)
{
    DataFrame df = create_sample_dataframe();
    EXPECT_THROW({ df({0, 20}, {"species"}).print(); }, std::out_of_range);
}

// Test slicing with Range
TEST(DataFrameTest, ValidSliceWithRange)
{
    DataFrame df = create_sample_dataframe();
    EXPECT_NO_THROW({
        df(Range(3), {"species"}).print();
    });
}

 
TEST(DataFrameTest, OutOfBoundsSliceWithRange)
{
    DataFrame df = create_sample_dataframe();
    EXPECT_THROW({ df(Range(20), {"species"}).print(); }, std::out_of_range);
}

//   Range and all columns
TEST(DataFrameTest, ValidSliceWithRangeAllColumns)
{
    DataFrame df = create_sample_dataframe();
    EXPECT_NO_THROW({
        df(Range(3)).print();
    });
}

//   out-of-bounds Range with all columns
TEST(DataFrameTest, OutOfBoundsRangeAllColumns)
{
    DataFrame df = create_sample_dataframe();
    EXPECT_THROW({ df(Range(10)).print(); }, std::out_of_range);
}
