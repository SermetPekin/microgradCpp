
/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#include "micrograd.hpp"
#include "value.hpp"
#include "mlp.hpp"
using namespace microgradCpp;
int main()
{
    // DatasetType dataset = get_iris();
    DataFrame df;
    df.from_csv("./data/iris.csv");
   df.normalize();
    df.encode_column("variety");
    df.print();
    df.shuffle();
    df.print();
    // stop();
    // return 0;
    // shuffle(dataset);
    double TRAIN_SIZE{0.8};
    // Create MLP model
    // Input: 4 features, hidden layers: [7,7], output: 3 classes
    // Define the model and hyperparameters
    // MLP model(4, {10, 10, 3});
    MLP model(4, {16, 16, 3});
    double learning_rate = 0.001;
    int epochs = 300;
    // Train and evaluate the model
    train_eval(df, TRAIN_SIZE, model, learning_rate, epochs);
    return 0;
}
/*
   // Initialize Adam optimizer
    AdamOptimizer optimizer(params, learning_rate);
    // Train and evaluate the model
    // train_eval(dataset, TRAIN_SIZE, model, learning_rate, epochs);
    train_eval(dataset, TRAIN_SIZE, model, optimizer, epochs);
    */
/*
Notes
-----------
g++ -std=c++17 -Iinclude -O2 -o main easy_df.cpp
// or
make run
*/