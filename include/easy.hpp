#ifndef EASY_HPP
#define EASY_HPP

/*
MIT License

Copyright (c) [2024] Sermet Pekin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <iostream>
#include <random>
#include <utility>

#include <iostream>
#include <random>
#include <utility>

// #include "micrograd.hpp"
#include "types.hpp"
#include "loss.hpp"
#include "mlp.hpp"
#include "sgd.hpp"
#include "dataprocessor.hpp"
#include "datasetType.hpp"
#include "console_utils.hpp"
#include "data_utils.hpp"

#include "types.hpp"
using namespace microgradCpp;

inline DatasetType get_iris2() // ok
{
    DataProcessor processor;

    vv_string data2 = processor.load_and_process("./data/iris.csv", true);

    vv_double converted_data2 = processor.convert_to_double_with_encoding_(data2);

    DatasetType dataset = convert_to_dataset(converted_data2);

    return dataset;
}

inline DatasetType get_iris() // ok
{
    return get_iris2();
}

inline DatasetType get_iris1() // problem
{
    // Load Iris dataset
    std::vector<std::vector<std::shared_ptr<Value>>> inputs;
    std::vector<std::vector<std::shared_ptr<Value>>> targets;
    std::string url = "./data/iris.csv";

    IrisLoader::load_iris(url, inputs, targets);

    DatasetType dataset;
    for (size_t i = 0; i < inputs.size(); ++i)
    {
        dataset.push_back({inputs[i], targets[i]});
    }
    return dataset;
}
inline void shuffle(DatasetType &dataset)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(42); // A fixed seed for reproducibility
    std::shuffle(dataset.begin(), dataset.end(), gen);
}
inline void train_test_split(
    const DatasetType &dataset,
    double TRAIN_SIZE,
    ColRows &train_inputs,
    ColRows &train_targets,
    ColRows &test_inputs,
    ColRows &test_targets)
{

    size_t train_size = static_cast<size_t>(dataset.size() * TRAIN_SIZE);

    for (size_t i = 0; i < train_size; ++i)
    {
        train_inputs.push_back(dataset[i].first);
        train_targets.push_back(dataset[i].second);
    }
    for (size_t i = train_size; i < dataset.size(); ++i)
    {
        test_inputs.push_back(dataset[i].first);
        test_targets.push_back(dataset[i].second);
    }
}


inline void train_eval(const DatasetType &dataset, double TRAIN_SIZE, MLP &model, double lr = 0.01, int epochs = 100)
{

    // Split into train and test sets (80-20 split)
    ColRows train_inputs, train_targets;
    ColRows test_inputs, test_targets;

    train_test_split(dataset, TRAIN_SIZE, train_inputs, train_targets, test_inputs, test_targets);

    // Create SGD optimizer with a learning rate of 0.005
    SGD optimizer(lr);

    // Validate dataset and model
    if (!validate_dataset_and_model(dataset, model, TRAIN_SIZE))
    {

        epic_failure_exit("data set validation failed...");
    }

    // int epochs = 100;
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
}

#endif // EASY_HPP