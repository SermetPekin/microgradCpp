
/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#include "micrograd.hpp"
using namespace microgradCpp;
int main()
{
    DatasetType dataset = get_iris();
    shuffle(dataset);
    double TRAIN_SIZE{0.8};
    // Create MLP model
    // Input: 4 features, hidden layers: [7,7], output: 3 classes
    // Define the model and hyperparameters
    MLP model(4, {10, 10, 3});
    double learning_rate = 0.01;
    int epochs = 100;
    // Train and evaluate the model
    train_eval(dataset, TRAIN_SIZE, model, learning_rate, epochs);
    return 0;
}
/*
Notes
-----------
g++ -std=c++17 -Iinclude -O2 -o main main_easy.cpp
// or
make run
*/