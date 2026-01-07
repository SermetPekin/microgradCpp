/*
 * MIT License
 * Copyright (c) 2024 Sermet Pekin
 *
 * Advanced example: DataFrame + Adam optimizer with interactive configuration.
 * Demonstrates complete ML pipeline with data preprocessing and optimization control.
 * Compile: g++ -std=c++17 -Iinclude -O2 -o main main.cpp
 */
#include "micrograd.hpp"
using namespace microgradCpp;

int main()
{
    // Load and preprocess data
    DataFrame df;
    df.from_csv("./data/iris.csv");
    df.normalize();
    df.encode_column("variety");
    df.shuffle();

    // Create model
    MLP model(4, {10, 10, 3});
    auto params = model.parameters();

    // Interactive configuration
    int epochs;
    std::cout << "Enter number of epochs: ";
    std::cin >> epochs;

    // Train with Adam optimizer
    AdamOptimizer optimizer(params, 0.01);
    train_eval(df, 0.8, model, optimizer, epochs);

    return 0;
}