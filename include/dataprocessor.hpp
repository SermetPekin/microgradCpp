
#ifndef DataProcessor_HPP

#define DataProcessor_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "csv.hpp"
#include "tensor_basic.hpp"
#include "schema.hpp"
#include "data_utils.hpp"

#include <cctype>
#include <iostream>
#include <sstream>

#include <string>
#include <functional>
#include <string>
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <string>
#include "types.hpp"
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

// Simple hash function for strings
inline size_t hash_string(const std::string &str)
{
    return std::hash<std::string>{}(str);
}

std::string trim(const std::string &str);
std::vector<std::vector<double>> convert_to_double(const std::vector<std::vector<std::string>> &data, const Schema &schema);

// Helper method to trim strings
inline std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

inline bool is_numeric(const std::string &str)
{
    // Trim quotes and whitespace
    auto start = str.find_first_not_of(" \"");
    auto end = str.find_last_not_of(" \"");
    if (start == std::string::npos)
    {
        return false;
    }
    std::string trimmed = str.substr(start, end - start + 1);

    // Handle empty strings after trimming
    if (trimmed.empty())
    {
        return false;
    }

    // Check if the trimmed string represents a valid number
    bool decimal_point = false;
    bool has_digit = false;

    for (size_t i = 0; i < trimmed.size(); ++i)
    {
        if (std::isdigit(trimmed[i]))
        {
            has_digit = true;
        }
        else if (trimmed[i] == '.' && !decimal_point)
        {
            decimal_point = true; // Allow one decimal point
        }
        else if (i == 0 && (trimmed[i] == '-' || trimmed[i] == '+'))
        {
            // Allow a leading '-' or '+' sign
            continue;
        }
        else
        {
            return false; // Any other character makes it non-numeric
        }
    }

    return has_digit;
}

class DataProcessor
{
public:
    Tensor load_and_process(const std::string &file_path, const Schema &schema)
    {
        CSVLoader loader;
        auto raw_data = loader.load_csv(file_path);

        // Convert data to double or perform one-hot encoding
        std::vector<std::vector<double>> processed_data = convert_to_double(raw_data, schema);

        return Tensor(processed_data);
    }

    vv_string load_and_process(const std::string &file_path, bool skip_header = false,
                               char delimiter = ',')
    {

        return CSVLoader::load_csv(file_path, skip_header = skip_header, delimiter = delimiter);
    }

 
    vv_double convert_to_double_with_encoding_(const vv_string &data, bool has_header = false)
    {
        vv_double x = convert_to_double_with_encoding(data, has_header);
        return x;
    }
 
 

private:
    // Hash function to convert strings to numerical values
    size_t hash_string(const std::string &str)
    {
        return std::hash<std::string>{}(str);
    }
};

#endif // DataProcessor_HPP
