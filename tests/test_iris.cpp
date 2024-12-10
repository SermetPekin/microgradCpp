/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#include "micrograd.hpp"

#include <set>
#include <iostream>
#include <gtest/gtest.h>
 
TEST(IrisTest, LoadAndVerifyClasses) {
    std::vector<std::vector<std::shared_ptr<Value>>> inputs;
    std::vector<std::vector<std::shared_ptr<Value>>> targets;

    std::string url = "./data/iris.csv";

    try {
         IrisLoader::load_iris(url, inputs, targets);

         std::set<std::vector<double>> unique_classes;

        for (const auto& target : targets) {
            std::vector<double> class_vector;
            for (const auto& val : target) {
                class_vector.push_back(val->data);
            }
            unique_classes.insert(class_vector);
        }

         ASSERT_EQ(unique_classes.size(), 3) << "There should be exactly 3 unique target classes";

         for (const auto& cls : unique_classes) {
            ASSERT_FALSE(cls.empty()) << "One of the target classes is empty";
        }

    
    } catch (const std::exception& e) {
        FAIL() << "Error loading Iris dataset: " << e.what();
    }
}

 