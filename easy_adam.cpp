
/*
 * MIT License
 * Copyright (c) 2024 Sermet Pekin
 *
 * Adam optimizer example: Same setup as easy.cpp but using AdamOptimizer.
 * Compile: g++ -std=c++17 -Iinclude -O2 -o easy_adam easy_adam.cpp
 */
#include "micrograd.hpp"
using namespace microgradCpp;

int main()
{
    // Load and shuffle the Iris dataset
    DatasetType dataset = get_iris();
    shuffle(dataset);

    // Create a simple MLP: 4 inputs -> [10, 10] hidden -> 3 outputs
    MLP model(4, {10, 10, 3});

    // Collect parameters and initialize Adam optimizer
    auto params = model.parameters();
    AdamOptimizer optimizer(params, 0.01);

    // Train with Adam optimizer
    train_eval(dataset, 0.8, model, optimizer, 100);

    return 0;
}