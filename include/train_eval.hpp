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

// The updated train_eval function
inline void train_test_split(
    const DataFrame &df,
    double TRAIN_SIZE,
    ColRows &train_inputs,
    ColRows &train_targets,
    ColRows &test_inputs,
    ColRows &test_targets)
{

    // size_t train_size = static_cast<size_t>(df.size() * TRAIN_SIZE);

    DatasetType dataset;

    dataset = convert_to_dataset(df);
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

/*========================      train_eval DataFrame      =================================================*/
inline void train_eval(DataFrame &df, double train_size, MLP &model, double lr = 0.01, int epochs = 100)
{

    ColRows train_inputs, train_targets;
    ColRows test_inputs, test_targets;

    train_test_split(df, train_size, train_inputs, train_targets, test_inputs, test_targets);

    // Create SGD optimizer
    SGD optimizer(lr);

    auto start = std::chrono::high_resolution_clock::now();

    print(test_targets);
    print(train_targets);
    print(train_inputs);
    print(test_inputs);
    // throw std::runtime_error("[ok]");

    // return std::EXIT_SUCCESS ;

    // Training loop
    for (int epoch = 0; epoch < epochs; ++epoch)
    {
        double total_loss = 0.0;
        std::cout << "[ Here it is ] ...Epoch " << epoch + 1 << "/" << epochs << ", Loss: " << total_loss / train_inputs.size() << std::endl;

        size_t NUM_Training = train_inputs.size();

        for (size_t i = 0; i < NUM_Training; ++i)
        {

            // Forward pass (training=true)
            auto predictions = model.forward(train_inputs[i], true);
            std::cout << "[ Here it is ] predictions ";

            // int num_classes = predictions.size();                                  // Number of classes inferred from predictions
            // v_shared_Value target = one_hot_encode(train_targets[i], num_classes); // Convert class index to one-hot

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

            size_t NUM_Test = test_inputs.size();

            for (size_t i = 0; i < NUM_Test; ++i)
            {
                auto predictions = model.forward(test_inputs[i], false);

                // Find predicted class
                int predicted_class = std::distance(predictions.begin(), std::max_element(predictions.begin(), predictions.end()));

                // Check if prediction matches the target
                if (test_targets[i][predicted_class]->data == 1.0)
                {
                    correct++;
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
