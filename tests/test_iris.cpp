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

 