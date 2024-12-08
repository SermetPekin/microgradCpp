#pragma once

#include <iostream>
#include <chrono>
#include <vector>
#include "dataframe.hpp"
#include "mlp.hpp"
#include "sgd.hpp"
#include "loss.hpp"
#include "dataframe.hpp"
#include "types.hpp"
#include "datasetType.hpp"
#include "value.hpp"
#include "mlp.hpp"
#include "adam.hpp"
#include "easy.hpp"
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
using namespace microgradCpp;

// Function to split the DataFrame into train and test sets
// inline void train_test_split(DataFrame &df, double train_size,
//                              std::vector<std::vector<double>> &train_inputs,
//                              std::vector<std::vector<double>> &train_targets,
//                              std::vector<std::vector<double>> &test_inputs,
//                              std::vector<std::vector<double>> &test_targets)
// {
//     size_t total_rows = 150; // TODO
//     size_t train_rows = static_cast<size_t>(total_rows * train_size);

//     DataFrame train_df = df.rows(Range(0, train_rows));
//     DataFrame test_df = df.rows(Range(train_rows, total_rows));

//     // Assuming inputs are all columns except the last one, and targets are in the last column
//     // train_inputs = train_df.get_inputs();   // Extract inputs from train DataFrame
//     // train_targets = train_df.get_targets(); // Extract targets from train DataFrame

//     // test_inputs = test_df.get_inputs();   // Extract inputs from test DataFrame
//     // test_targets = test_df.get_targets(); // Extract targets from test DataFrame
// }

inline std::vector<std::shared_ptr<Value>> one_hot_encodeLocal(int class_index, int num_classes)
{
    std::vector<std::shared_ptr<Value>> target(num_classes, std::make_shared<Value>(0.0));
    target[class_index] = std::make_shared<Value>(1.0);
    return target;
}

inline void shuffleLocal(DatasetType &dataset)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    // gen.seed(42); // A fixed seed for reproducibility
    std::shuffle(dataset.begin(), dataset.end(), gen);
}

// The updated train_eval function
inline void train_test_split(    const DataFrame &df,
    double TRAIN_SIZE,
    ColRows &train_inputs,
    ColRows &train_targets,
    ColRows &test_inputs,
    ColRows &test_targets)
{

    // size_t train_size = static_cast<size_t>(df.size() * TRAIN_SIZE);

    DatasetType dataset;

    dataset = convert_to_dataset(df);
    // shuffleLocal(dataset);

    size_t train_size = static_cast<size_t>(dataset.size() * TRAIN_SIZE);

    std::cout << "train_size : " << train_size;
    // stop();
    int num_classes = 3;
    for (size_t i = 0; i < train_size; ++i)
    {
        train_inputs.push_back(dataset[i].first);

        // train_targets.push_back(dataset[i].second);
        int class_index = static_cast<int>(dataset[i].second[0]->data);
        train_targets.push_back(one_hot_encodeLocal(class_index, num_classes));
    }
    for (size_t i = train_size; i < dataset.size(); ++i)
    {
        test_inputs.push_back(dataset[i].first);
        // test_targets.push_back(dataset[i].second);

        int class_index = static_cast<int>(dataset[i].second[0]->data);
        test_targets.push_back(one_hot_encodeLocal(class_index, num_classes));
    }
}
inline void print(const ColRows &colrows)
{
    for (size_t i = 0; i < colrows.size(); ++i)
    {
        std::cout << "Row " << i << ": ";
        for (const auto &cell : colrows[i])
        {
            if (cell)
            {
                std::cout << cell->data << " ";
            }
            else
            {
                std::cout << "NaN ";
            }
        }
        std::cout << "\n";
    }
}

// std::vector<std::shared_ptr<Value>> one_hot_encode(int class_index, int num_classes)
// {

//     std::vector<std::shared_ptr<Value>> target(num_classes, std::make_shared<Value>(0.0));

//     target[class_index] = std::make_shared<Value>(1.0);

//     return target;
// }

inline void train_eval(DatasetType &dataset, double TRAIN_SIZE, MLP &model, AdamOptimizer &optimizer, int epochs = 100)
{

    // Split into train and test sets (80-20 split)
    ColRows train_inputs, train_targets;
    ColRows test_inputs, test_targets;

    train_test_split(dataset, TRAIN_SIZE, train_inputs, train_targets, test_inputs, test_targets);

    // Create SGD optimizer with a learning rate of 0.005
    // SGD optimizer(lr);

    double best_loss = std::numeric_limits<double>::infinity();
    int patience = 150, patience_counter = 0;

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
            optimizer.zero_grad();
            loss->backward();

            // Update weights
            optimizer.step();
        }

       // std::cout << "Epoch " << epoch + 1 << "/" << epochs << ", Loss: " << total_loss / train_inputs.size() << std::endl;

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

inline void train_eval(DataFrame &df, double TRAIN_SIZE, MLP &model, AdamOptimizer &optimizer, int epochs = 100)
{

    ColRows train_inputs, train_targets;
    ColRows test_inputs, test_targets;

    train_test_split(df, TRAIN_SIZE, train_inputs, train_targets, test_inputs, test_targets);

    // Create SGD optimizer
    // SGD optimizer(lr);

    double best_loss = std::numeric_limits<double>::infinity();
    int patience = 150, patience_counter = 0;

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
            optimizer.zero_grad();
            loss->backward();

            // Update weights
            optimizer.step();
        }

      //  std::cout << "Epoch " << epoch + 1 << "/" << epochs << ", Loss: " << total_loss / train_inputs.size() << std::endl;

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

/*========================      train_eval DataFrame      =================================================*/
inline void train_eval(const DataFrame &df, double train_size, MLP &model, double lr = 0.01, int epochs = 100)
{

    ColRows train_inputs, train_targets;
    ColRows test_inputs, test_targets;

    train_test_split(df, train_size, train_inputs, train_targets, test_inputs, test_targets);

    // Create SGD optimizer
    SGD optimizer(lr);

    auto start = std::chrono::high_resolution_clock::now();

    v_string Dummy = {"aa", "bb"};
    display_data(train_inputs, train_targets, Dummy);
    std::cout << "=================================\n";
    display_data(test_inputs, test_targets, Dummy);
    // stop("...");

    // Training loop
    for (int epoch = 0; epoch < epochs; ++epoch)
    {
        double total_loss = 0.0;

        size_t NUM_Training = train_inputs.size();

        for (size_t i = 0; i < NUM_Training; ++i)
        {

            // Forward pass (training=true)
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

        // Evaluate test accuracy every 10 epochs and on the last epoch
        if (epoch % 10 == 0 || epoch == epochs - 1)
        {
            int correct = 0;

            size_t NUM_Test = test_inputs.size();

            for (size_t i = 0; i < NUM_Test; ++i)
            {
                auto predictions = model.forward(test_inputs[i], false);

                // Find predicted class
                int predicted_class = std::distance(predictions.begin(), std::max_element(predictions.begin(), predictions.end()));

                // Check if prediction matches the target
                // if (test_targets[i][predicted_class]->data == 1.0)
                // if (test_targets[i][predicted_class]->data == 1.0)
                // {
                //     correct++;
                // }

                if (predicted_class >= 0 && predicted_class < test_targets[i].size())
                {
                    // if (test_targets[i][predicted_class]->data == 1.0)
                    // {
                    //     correct++;
                    // }

                    constexpr double EPSILON = 1e-6;
                    if (std::abs(test_targets[i][predicted_class]->data - 1.0) < EPSILON)
                    {
                        correct++;
                    }
                }
            }

            double accuracy = static_cast<double>(correct) / test_inputs.size();
            std::cout << "Epoch " << epoch + 1 << ": Test Accuracy = " << accuracy * 100.0 << "%" << std::endl;

            if (epoch == epochs - 1)
            {
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                std::cout << "Duration: " << duration.count() << " seconds" << std::endl;
            }
        }
    }
}
