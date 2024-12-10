
#ifndef LINEAR_HPP
#define LINEAR_HPP
/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#include "value.hpp"
#include "mlp.hpp"
#include "types.hpp"
#include <vector>
#include <random>
#include <memory>
#include <iostream>
#include <iostream>
using namespace microgradCpp ;
inline std::vector<std::shared_ptr<Value>> softmaxLocal(const std::vector<std::shared_ptr<Value>> &inputs)
{
    // sum_exp = sum of exp(input)
    auto sum_exp = std::make_shared<Value>(0.0);
    for (auto &inp : inputs)
    {
        auto e = inp->exp(); // e = exp(inp)
        sum_exp = sum_exp + e;
    }
    // each output = exp(input) / sum_exp
    std::vector<std::shared_ptr<Value>> outputs;
    for (auto &inp : inputs)
    {
        auto e = inp->exp();
        auto prob = e / sum_exp;
        outputs.push_back(prob);
    }
    return outputs;
}
class Linear
{
public:
    int in_features, out_features;
    std::vector<std::vector<std::shared_ptr<Value>>> weights;
    std::vector<std::shared_ptr<Value>> biases;
    Linear(int in_features, int out_features)
        : in_features(in_features), out_features(out_features)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(-1.0, 1.0);
        for (int i = 0; i < out_features; ++i)
        {
            std::vector<std::shared_ptr<Value>> row;
            for (int j = 0; j < in_features; ++j)
            {
                // row.push_back(std::make_shared<Value>(dist(gen), "w"));
                row.push_back(std::make_shared<Value>((rand() % 1000) / 1000.0 - 0.5, "w"));
            }
            weights.push_back(row);
            biases.push_back(std::make_shared<Value>(dist(gen), "b"));
        }
    }
    //  weights[i] = std::make_shared<Value>((rand() % 1000) / 1000.0 - 0.5);  // Values between -0.5 and 0.5
    // Forward pass
    std::vector<std::shared_ptr<Value>> forward(const std::vector<std::shared_ptr<Value>> &inputs)
    {
        if (inputs.empty())
        {
            throw std::runtime_error("Empty input provided to Linear layer forward pass.");
        }
        std::vector<std::shared_ptr<Value>> outputs;
        for (int i = 0; i < out_features; ++i)
        {
            auto activation = biases[i];
            for (int j = 0; j < in_features; ++j)
            {
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
        // return outputs;
        return softmaxLocal(outputs);
    }
    // return softmax(activations);
    // Get all parameters (weights and biases)
    std::vector<std::shared_ptr<Value>> parameters() const
    {
        std::vector<std::shared_ptr<Value>> params;
        for (const auto &row : weights)
        {
            params.insert(params.end(), row.begin(), row.end());
        }
        params.insert(params.end(), biases.begin(), biases.end());
        return params;
    }
};
#endif // LINEAR_HPP