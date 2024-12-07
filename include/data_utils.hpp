
#ifndef DataUtils_HPP

#define DataUtils_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>

using vv_string = std::vector<std::vector<std::string>>;
using vv_double = std::vector<std::vector<double>>;

#include <fstream>
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


// #include <iostream>
// #include <vector>
// #include <string>

#endif // DataUtils_HPP
