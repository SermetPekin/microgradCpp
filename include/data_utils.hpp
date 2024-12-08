
#ifndef DataUtils_HPP

#define DataUtils_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "mlp.hpp"
#include "console_utils.hpp"
#include "types.hpp"
using namespace microgradCpp;

using vv_string = std::vector<std::vector<std::string>>;
using vv_double = std::vector<std::vector<double>>;



#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include "value.hpp"

inline 
void display_data(const ColRows& inputs, const ColRows& targets, const std::vector<std::string>& column_names ) {
    // Print column headers
    for (const auto& col_name : column_names) {
        std::cout << std::setw(15) << std::left << col_name;
    }
    std::cout << std::setw(15) << std::left << "target";
    std::cout << "\n";

    // Print separator line
    for (size_t i = 0; i < column_names.size() + 1; ++i) {
        std::cout << std::setw(15) << std::setfill('-') << "" << std::setfill(' ');
    }
    std::cout << "\n";

    // Print rows of data
    for (size_t i = 0; i < inputs.size(); ++i) {
        for (const auto& value : inputs[i]) {
            std::cout << std::setw(15) << std::left << value->data;
        }

        // Print the target
        for (const auto& target : targets[i]) {
            std::cout << std::setw(15) << std::left << target->data;
        }

        std::cout << "\n";
    }

    // Print final separator line
    std::cout << "========================\n";
}


static inline v_shared_Value one_hot_encode(int class_index, int num_classes)
{

    v_shared_Value target(num_classes, std::make_shared<Value>(0.0));

    target[class_index] = std::make_shared<Value>(1.0);

    return target;
}

inline void log_model_info(const std::vector<int> &layer_sizes,
                           size_t input_features,
                           size_t output_targets,
                           size_t train_size,
                           size_t test_size,
                           double TRAIN_RATIO);

template <class T>
inline void write_to_csv(const std::vector<std::vector<T>> &data, const std::string &filename)
{
    std::ofstream file(filename);

    // Check if the file is open
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    for (const auto &row : data)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            file << row[i];

            if (i < row.size() - 1)
            {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
    std::cout << "Data successfully written to " << filename << "\n";
}
inline std::unordered_map<std::string, double> create_target_encoding(const std::vector<std::string> &target_column)
{
    std::unordered_map<std::string, double> encoding_map;
    double index = 0.0;

    for (const auto &value : target_column)
    {
        if (encoding_map.find(value) == encoding_map.end())
        {
            encoding_map[value] = index++;
        }
    }

    return encoding_map;
}

inline vv_double convert_to_double_with_encoding(const vv_string &data, bool has_header = false)
{
    vv_double converted_data;
    size_t start_index = has_header ? 1 : 0;

    // Extract the target column (assuming the last column is the target)
    std::vector<std::string> target_column;
    for (size_t i = start_index; i < data.size(); ++i)
    {
        if (data[i].empty())
        {
            std::cerr << "Warning: Skipping empty row at index " << i << std::endl;
            continue;
        }

        target_column.push_back(data[i].back());
    }

    if (target_column.empty())
    {
        throw std::runtime_error("Error: No target column found in the data.");
    }

    // Create target encoding map
    auto target_encoding_map = create_target_encoding(target_column);

    for (size_t i = start_index; i < data.size(); ++i)
    {
        if (data[i].empty())
        {
            std::cerr << "Warning: Skipping empty row at index " << i << std::endl;
            continue;
        }

        std::vector<double> converted_row;

        // Convert all but the last column (features) to double
        for (size_t j = 0; j < data[i].size() - 1; ++j)
        {
            try
            {
                converted_row.push_back(std::stod(data[i][j]));
            }
            catch (const std::invalid_argument &)
            {
                std::cerr << "Warning: Invalid numeric value in row " << i << ", column " << j << ": '" << data[i][j] << "', defaulting to 0.0" << std::endl;
                converted_row.push_back(0.0); // Fallback for invalid numeric conversion
            }
        }

        // Encode the target column using the encoding map
        std::string target_value = data[i].back();
        if (target_encoding_map.find(target_value) == target_encoding_map.end())
        {
            throw std::runtime_error("Error: Target value '" + target_value + "' not found in encoding map.");
        }
        converted_row.push_back(target_encoding_map[target_value]);

        converted_data.push_back(converted_row);
    }

    return converted_data;
}

// Function to validate the dataset and model
inline bool validate_dataset_and_model(const DatasetType &dataset, const MLP &model, double TRAIN_RATIO)
{
    if (dataset.empty())
    {
        display_header("Error: Dataset is empty!");
        return false;
    }

    size_t input_size = model.input_size();
    size_t output_size = model.output_size();

    int sz = dataset.size();

    log_model_info(model.get_layer_sizes(), input_size, output_size, sz, sz, TRAIN_RATIO);

    display_shapes("Model", input_size, output_size);

    for (size_t i = 0; i < dataset.size(); ++i)
    {
        const auto &inputs = dataset[i].first;
        const auto &targets = dataset[i].second;

        if (inputs.size() != input_size)
        {
            display_header("Input Size Mismatch Detected");
            std::cerr << "❌ Sample " << i << ": Expected input size "
                      << input_size << ", but got " << inputs.size() << "\n";
            std::cerr << "🔹 Input Shape: [" << format_shape(inputs.size(), 1) << "]\n";
            return false;
        }

        if (targets.size() != output_size)
        {
            display_header("Target Size Mismatch Detected");
            std::cerr << "❌ Sample " << i << ": Expected target size "
                      << output_size << ", but got " << targets.size() << "\n";
            std::cerr << "🔸 Target Shape: [" << format_shape(targets.size(), 1) << "]\n";
            return false;
        }

        // Check if inputs are valid (not null)
        for (size_t j = 0; j < inputs.size(); ++j)
        {
            if (!inputs[j])
            {
                display_header("Null Input Value Detected");
                std::cerr << "❌ Sample " << i << ", Feature " << j << ": Null input value\n";
                return false;
            }
        }

        // Check if targets are valid (not null)
        for (size_t j = 0; j < targets.size(); ++j)
        {
            if (!targets[j])
            {
                display_header("Null Target Value Detected");
                std::cerr << "❌ Sample " << i << ", Target " << j << ": Null target value\n";
                return false;
            }
        }
    }

    std::cout << "✅ All checks passed! Dataset and model are ready for training.\n";
    std::cout << "----------------------------------------------------\n";
    return true;
}

inline void log_model_info(const std::vector<int> &layer_sizes,
                           size_t input_features,
                           size_t output_targets,
                           size_t train_size,
                           size_t test_size,
                           double TRAIN_RATIO)
{
    std::cout << "\n📊  Model and Dataset Info\n";
    std::cout << "===========================================\n";

    // Log model shape
    std::cout << "🧠 Model Shape:\n";
    std::cout << "   ┌─────────────────────────┐\n";
    std::cout << "   │ Input: " << input_features << " features       │\n";
    for (size_t i = 0; i < layer_sizes.size(); ++i)
    {
        std::cout << "   ├── Layer " << i + 1 << ": " << layer_sizes[i] << " neurons    │\n";
    }
    std::cout << "   └─────────────────────────┘\n";
    std::cout << "   ⮕ Output: " << output_targets << " targets\n";

    // Log dataset info
    std::cout << "\n📂 Dataset Info:\n";
    std::cout << "   ┌────────────────────────┐\n";
    std::cout << "   │ Training Samples:  " << train_size * TRAIN_RATIO << " │\n";
    std::cout << "   │ Testing Samples:    " << test_size * (1 - TRAIN_RATIO) << " │\n";
    std::cout << "   └────────────────────────┘\n";
    std::cout << "        Train ratio       " << TRAIN_RATIO << "           \n";
    std::cout << "===========================================\n\n";
}

// #include <iostream>
// #include <vector>
// #include <string>

#endif // DataUtils_HPP
