// MIT License

// Copyright (c) [2024] Sermet Pekin

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// */

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
