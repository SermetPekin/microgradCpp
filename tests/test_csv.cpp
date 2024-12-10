/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/

#include "micrograd.hpp"
#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <stdexcept>

inline std::string iris_path= "./data/iris.csv"; 

// Test if the CSV file is loaded successfully
TEST(CSVLoaderTest, LoadCSVSuccess) {
    try {
        
        auto data = CSVLoader::load_csv(iris_path , true);

         
        ASSERT_FALSE(data.empty()) << "CSV data should not be empty.";

         EXPECT_EQ(data.size(), 150) << "Unexpected number of rows in the dataset.";

         EXPECT_EQ(data[0].size(), 5) << "Unexpected number of columns in the dataset.";

    } catch (const std::exception& e) {
        FAIL() << "Exception thrown during CSV loading: " << e.what();
    }
}
 

//  loading CSV without header skipping
TEST(CSVLoaderTest, LoadCSVNoHeaderSkip) {
    try {
        auto data = CSVLoader::load_csv( iris_path , false);

    
        ASSERT_FALSE(data.empty()) << "CSV data should not be empty.";
        EXPECT_EQ(data[0][0], "sepal.length") << "Expected header row as the first row.";
    } catch (const std::exception& e) {
        FAIL() << "Exception thrown during CSV loading: " << e.what();
    }
}


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
 