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
#ifndef LINEAR_HPP
#define LINEAR_HPP

#include "value.hpp"
#include <vector>
#include <random>
#include <memory>
#include <iostream>

class Linear {
public:
    int in_features, out_features;
    std::vector<std::vector<std::shared_ptr<Value>>> weights;
    std::vector<std::shared_ptr<Value>> biases;

    Linear(int in_features, int out_features)
        : in_features(in_features), out_features(out_features) {
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(-1.0, 1.0);

        for (int i = 0; i < out_features; ++i) {
            std::vector<std::shared_ptr<Value>> row;
            for (int j = 0; j < in_features; ++j) {
                row.push_back(std::make_shared<Value>(dist(gen), "w"));
            }
            weights.push_back(row);
            biases.push_back(std::make_shared<Value>(dist(gen), "b"));
        }
    }

    // Forward pass
    std::vector<std::shared_ptr<Value>> forward(const std::vector<std::shared_ptr<Value>>& inputs) {
        std::vector<std::shared_ptr<Value>> outputs;
        for (int i = 0; i < out_features; ++i) {
            auto activation = biases[i];
            for (int j = 0; j < in_features; ++j) {
                activation = activation + (weights[i][j] * inputs[j]);
            }
            outputs.push_back(activation);
        }

            // Debugging: Print outputs
    /*    std::cout << "Linear Layer Activations: ";
        for (const auto& output : outputs) {
            std::cout << output->data << " ";
        }
        std::cout << std::endl;
*/
        return outputs;
    }

        // Get all parameters (weights and biases)
    std::vector<std::shared_ptr<Value>> parameters() const {
        std::vector<std::shared_ptr<Value>> params;
        for (const auto& row : weights) {
            params.insert(params.end(), row.begin(), row.end());
        }
        params.insert(params.end(), biases.begin(), biases.end());
        return params;
    }

};

#endif // LINEAR_HPP

