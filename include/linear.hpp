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

