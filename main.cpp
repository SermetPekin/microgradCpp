
#include <iostream>
#include <random>
#include <utility>

#include "micrograd.hpp"

using namespace microgradCpp;

/*

g++ -g -o main main.cpp

g++ -g -std=c++17 -Iinclude -O2 -o main main.cpp

clang++  -std=c++17 -Iinclude  -g -o  main main.cpp

(lldb) breakpoint set --file main.cpp --line 75
(lldb) run

(lldb) bt
(lldb) print some_variable



*/

#include <iostream>
#include <vector>
#include <memory>
// #include "value.hpp"  // Assuming Value class is defined here
// #include "mlp.hpp"    // Assuming MLP class is defined here

// using ColRows = std::vector<std::vector<std::shared_ptr<Value>>>;
// using DatasetType = std::vector<std::pair<std::vector<std::shared_ptr<Value>>, std::vector<std::shared_ptr<Value>>>>;

#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <sstream>
#include "value.hpp" // Assuming Value class is defined here
#include "mlp.hpp"   // Assuming MLP class is defined here

// using ColRows = std::vector<std::vector<std::shared_ptr<Value>>>;
// using DatasetType = std::vector<std::pair<std::vector<std::shared_ptr<Value>>, std::vector<std::shared_ptr<Value>>>>;

int main()
{

    DatasetType dataset = get_iris2();

    shuffle(dataset);

    double TRAIN_SIZE{0.8};

    // Split into train and test sets (80-20 split)
    ColRows train_inputs, train_targets;
    ColRows test_inputs, test_targets;

    train_test_split(dataset, TRAIN_SIZE, train_inputs, train_targets, test_inputs, test_targets);

    // Create MLP model
    // Input: 4 features, hidden layers: [7,7], output: 3 classes
    MLP model(4, {7, 7, 3});

    // Validate dataset and model
    if (!validate_dataset_and_model(dataset, model, TRAIN_SIZE))
    {
        std::cerr << "Validation failed. Exiting." << std::endl;
        return 1;
    }

    // Create SGD optimizer with a learning rate of 0.005
    SGD optimizer(0.01);

    int epochs = 100;

    int x = 0;
    std::cout << "Epoch: " << epochs << ", Sample: " << x << std::endl;

    // Validate input size
    std::cout << "Input size: " << train_inputs[x].size() << std::endl;
    std::cout << "Target size: " << train_targets[x].size() << std::endl;

    for (int epoch = 0; epoch < epochs; ++epoch)
    {
        double total_loss = 0.0;

        // Training loop
        for (size_t i = 0; i < train_inputs.size(); ++i)
        {

            // Forward pass (training=true to possibly enable dropout or other training-specific behavior in MLP)
            auto predictions = model.forward(train_inputs[i], true);

            // Compute Cross-Entropy Loss
            auto loss = Loss::cross_entropy(predictions, train_targets[i]);
            total_loss += loss->data;

            // Backpropagation
            optimizer.zero_grad(model.parameters());
            loss->backward();

            // Update weights
            optimizer.step(model.parameters());
        }

        std::cout << "Epoch " << epoch + 1 << "/" << epochs << ", Loss: " << total_loss / train_inputs.size() << std::endl;

        // Evaluate test accuracy every 10 epochs and on the last epoch
        if (epoch % 10 == 0 || epoch == epochs - 1)
        {
            int correct = 0;
            for (size_t i = 0; i < test_inputs.size(); ++i)
            {
                // Forward pass in evaluation mode (e.g., no dropout)
                auto predictions = model.forward(test_inputs[i], false);

                // Find predicted class (the index with max value)
                int predicted_class = 0;
                double max_value = predictions[0]->data;
                for (size_t j = 1; j < predictions.size(); ++j)
                {
                    if (predictions[j]->data > max_value)
                    {
                        max_value = predictions[j]->data;
                        predicted_class = static_cast<int>(j);
                    }
                }

                // Check if prediction matches the target
                for (size_t j = 0; j < test_targets[i].size(); ++j)
                {
                    if (test_targets[i][j]->data == 1.0 && j == predicted_class)
                    {
                        correct++;
                        break;
                    }
                }
            }

            double accuracy = static_cast<double>(correct) / test_inputs.size();
            std::cout << "Epoch " << epoch + 1 << ": Test Accuracy = " << accuracy * 100.0 << "%" << std::endl;
        }
    }

    return 0;
}

/*
Notes
-----------

g++ -std=c++17 -Iinclude -O2 -o main main.cpp

// or
make run


*/
