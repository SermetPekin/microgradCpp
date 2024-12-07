
/*
 * evdscpp: An open-source data wrapper for accessing the EVDS API.
 * Author: Sermet Pekin
 *
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include <sstream>
#include <optional>
#include <cmath>
#include <typeindex>
#include <typeinfo> //   typeid
// #include "../extern/nlohmann/json.hpp"
#include "series.hpp"
#include "header.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <typeindex>
#include <typeinfo>

#include "dataframe.hpp"
namespace namespaceCpp
{

    inline bool is_numeric(const std::string &str)
    {
        std::string trimmed = trim(str);
        if (trimmed.empty())
            return false;

        char *end = nullptr;
        std::strtod(trimmed.c_str(), &end);
        return end == trimmed.c_str() + trimmed.size();
    }

    void DataFrame::from_csv(const std::string &filename, bool has_header = true, char delimiter = ',')
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Error opening file: " + filename);
        }

        std::string line;
        std::vector<std::string> column_names;
        bool is_first_line = true;

        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string cell;
            std::vector<std::string> cells;

            while (std::getline(ss, cell, delimiter))
            {
                cells.push_back(trim(cell));
            }

            if (is_first_line && has_header)
            {
                column_names = cells;
                for (const auto &col : column_names)
                {
                    columns[col] = Column();
                    column_types[col] = std::nullopt; // Initialize types as unknown
                }
                is_first_line = false;
            }
            else
            {
                if (!has_header && is_first_line)
                {
                    // If no header, create generic column names
                    for (size_t i = 0; i < cells.size(); ++i)
                    {
                        column_names.push_back("column_" + std::to_string(i));
                        columns[column_names[i]] = Column();
                        column_types[column_names[i]] = std::nullopt;
                    }
                    is_first_line = false;
                }

                for (size_t i = 0; i < cells.size(); ++i)
                {
                    const auto &col_name = column_names[i];
                    const std::string &value = cells[i];

                    if (is_numeric(value))
                    {
                        // Try to convert to double or long long
                        try
                        {
                            double num = std::stod(value);
                            columns[col_name].push_back(num);
                            column_types[col_name] = typeid(double);
                        }
                        catch (const std::invalid_argument &)
                        {
                            columns[col_name].push_back(value);
                            column_types[col_name] = typeid(std::string);
                        }
                    }
                    else
                    {
                        columns[col_name].push_back(value);
                        column_types[col_name] = typeid(std::string);
                    }
                }
            }
        }

        file.close();
    }

    // inline void DataFrame::from_csv(const std::string &filename, bool has_header, char delimiter)
    // {
    //     std::ifstream file(filename);
    //     if (!file.is_open())
    //     {
    //         throw std::runtime_error("Error: Could not open file " + filename);
    //     }

    //     std::string line;
    //     std::vector<std::string> column_names;
    //     bool first_row = true;
    //     size_t row_index = 0;

    //     while (std::getline(file, line))
    //     {
    //         std::stringstream ss(line);
    //         std::string cell;
    //         std::vector<std::string> row;

    //         // Split the line by the delimiter
    //         while (std::getline(ss, cell, delimiter))
    //         {
    //             row.push_back(cell);
    //         }

    //         if (first_row && has_header)
    //         {
    //             // Treat the first row as column names
    //             for (const auto &col_name : row)
    //             {
    //                 column_names.push_back(clean_colname(col_name));
    //                 columns[clean_colname(col_name)] = Column();
    //                 column_types[clean_colname(col_name)] = std::nullopt;
    //             }
    //             first_row = false;
    //         }
    //         else
    //         {
    //             // If no header, create default column names
    //             if (first_row && !has_header)
    //             {
    //                 for (size_t i = 0; i < row.size(); ++i)
    //                 {
    //                     std::string col_name = "col" + std::to_string(i);
    //                     column_names.push_back(col_name);
    //                     columns[col_name] = Column();
    //                     column_types[col_name] = std::nullopt;
    //                 }
    //                 first_row = false;
    //             }

    //             // Populate columns with data
    //             for (size_t i = 0; i < row.size(); ++i)
    //             {
    //                 const std::string &col_name = column_names[i];
    //                 std::string cell_value = trim(row[i]);

    //                 // Determine if the cell can be converted to a double
    //                 try
    //                 {
    //                     double num_value = std::stod(cell_value);
    //                     add_value_at(col_name, row_index, num_value);
    //                 }
    //                 catch (const std::invalid_argument &)
    //                 {
    //                     // If not, store it as a string
    //                     add_value_at(col_name, row_index, cell_value);
    //                 }
    //             }
    //             ++row_index;
    //         }
    //     }

    //     file.close();
    // }

    // namespace
}
