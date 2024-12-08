#ifndef DatasetType_hpp
#define DatasetType_hpp

#include <vector>
#include <memory>
#include <iostream>
#include "value.hpp"
#include "types.hpp"
#include "dataframe.hpp"
using namespace microgradCpp;
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
inline DatasetType convert_to_dataset(const vv_double &data, int target_column = -1)
{
    DatasetType dataset;

    for (const auto &row : data)
    {
        if (row.empty())
        {
            continue; // Skip empty rows
        }

        // Determine target column index
        int target_idx = (target_column == -1) ? row.size() - 1 : target_column;
        // Create inputs and targets
        std::vector<std::shared_ptr<Value>> inputs;
        std::vector<std::shared_ptr<Value>> targets;
        for (size_t i = 0; i < row.size(); ++i)
        {
            if (static_cast<int>(i) == target_idx)
            {
                targets.push_back(std::make_shared<Value>(row[i]));
            }
            else
            {
                inputs.push_back(std::make_shared<Value>(row[i]));
            }
        }

        // Add the pair to the dataset
        dataset.emplace_back(inputs, targets);
    }

    return dataset;
}

inline DatasetType convert_to_dataset(const DataFrame &df, int target_column = -1)
{
    DatasetType dataset;
    vv_double data = df.to_vv_double();


    for (const auto &row : data)
    {
        if (row.empty())
        {
            continue; // Skip empty rows
        }

        // Determine target column index
        int target_idx = (target_column == -1) ? row.size() - 1 : target_column;
        // Create inputs and targets
        std::vector<std::shared_ptr<Value>> inputs;
        std::vector<std::shared_ptr<Value>> targets;
        for (size_t i = 0; i < row.size(); ++i)
        {
            if (static_cast<int>(i) == target_idx)
            {
                targets.push_back(std::make_shared<Value>(row[i]));
            }
            else
            {
                inputs.push_back(std::make_shared<Value>(row[i]));
            }
        }

        // Add the pair to the dataset
        dataset.emplace_back(inputs, targets);
    }

    return dataset;
}

#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <sstream>

inline void write_dataset_to_csv(const DatasetType &dataset, const std::string &filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    // Write the header
    size_t num_features = dataset[0].first.size();
    for (size_t i = 0; i < num_features; ++i)
    {
        file << "Feature" << i + 1 << ",";
    }
    file << "Target" << std::endl;

    // Write the data
    for (const auto &sample : dataset)
    {
        const auto &features = sample.first;
        const auto &targets = sample.second;

        // Write features
        for (const auto &feature : features)
        {
            file << feature->data << ",";
        }

        // Write target (assuming one target per sample)
        if (!targets.empty())
        {
            file << targets[0]->data;
        }

        file << std::endl;
    }

    file.close();
    std::cout << "Dataset successfully written to " << filename << std::endl;
}

#endif // DatasetType_hpp
