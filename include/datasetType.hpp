#ifndef DatasetType_hpp
#define DatasetType_hpp

#include <vector>
#include <memory>
#include <iostream>
#include "value.hpp"
#include "types.hpp"
#include "dataframe.hpp"
using namespace microgradCpp;

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
