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
    MLP model(4, {10, 10,10,10,10, 3});
    double learning_rate = 0.01;
    int epochs = 1000;

    // Train and evaluate the model
    train_eval(dataset, TRAIN_SIZE, model, learning_rate, epochs);

    return 0;
}


/*
Notes
-----------

g++ -std=c++17 -Iinclude -O2 -o main main_no_cache.cpp

// or
make run

cache 
    14.6639 seconds
no cache 
    duration: 14.6024 seconds

MLP model(4, {10, 10,10,10,10, 3});
cache 
    37.644 seconds
no cache 
    37.5242 seconds

*/
