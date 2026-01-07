
/*
 * MIT License
 * Copyright (c) 2024 Sermet Pekin
 *
 * DataFrame example: Load CSV, normalize, encode categorical columns, then train.
 * Demonstrates the DataFrame API for data preprocessing.
 * Compile: g++ -std=c++17 -Iinclude -O2 -o easy_df easy_df.cpp
 */
#include "micrograd.hpp"
using namespace microgradCpp;

int main()
{
    // Load CSV and preprocess data
    DataFrame df;
    df.from_csv("./data/iris.csv");
    df.normalize();
    df.encode_column("variety");
    df.shuffle();

    // Create MLP and train with SGD optimizer
    MLP model(4, {10, 10, 3});
    train_eval(df, 0.8, model, 0.01, 100);

    return 0;
}