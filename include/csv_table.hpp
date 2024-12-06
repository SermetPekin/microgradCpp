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

#ifndef CSV_LOADER_HPP
#define CSV_LOADER_HPP

#include "column.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "types.hpp"

using namespace microgradCpp ; 


class CSVLoaderTable {
public:
    static Table load_csv(const std::string& filepath, const  v_string & column_names, const v_string & column_types) {
        // Check that the column names and types match
        if (column_names.size() != column_types.size()) {
            throw std::invalid_argument("Number of column names and types must match.");
        }

        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open CSV file: " + filepath);
        }

        // Create an empty table
        Table table;

        // Initialize vectors for each column
        vv_double numeric_columns(column_types.size());
       vv_string string_columns(column_types.size());

        // Read the file line by line
        std::string line;
        size_t line_count = 0;
        while (std::getline(file, line)) {
            line_count++;
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string cell;

            for (size_t col_index = 0; col_index < column_names.size(); ++col_index) {
                if (!std::getline(ss, cell, ',')) {
                    throw std::runtime_error("Missing data at line " + std::to_string(line_count));
                }

                trim(cell);

                if (column_types[col_index] == "double") {
                    try {
                        double value = std::stod(cell);
                        numeric_columns[col_index].push_back(value);
                    } catch (...) {
                        throw std::runtime_error("Error parsing cell as double: '" + cell + "' at line " + std::to_string(line_count));
                    }
                } else if (column_types[col_index] == "string") {
                    string_columns[col_index].push_back(cell);
                } else {
                    throw std::invalid_argument("Unsupported column type: " + column_types[col_index]);
                }
            }
        }

        file.close();

        // Add columns to the table
        for (size_t i = 0; i < column_names.size(); ++i) {
            if (column_types[i] == "double") {
                table.add_column(column_names[i], numeric_columns[i]);
            } else if (column_types[i] == "string") {
                table.add_column(column_names[i], string_columns[i]);
            }
        }

        return table;
    }

private:
    // Helper function to trim whitespace and quotes
    static void trim(std::string& s) {
        // Remove leading whitespace and quotes
        size_t start_pos = s.find_first_not_of(" \t\"");
        if (start_pos == std::string::npos) {
            s.clear();
            return;
        }
        s.erase(0, start_pos);

        // Remove trailing whitespace and quotes
        size_t end_pos = s.find_last_not_of(" \t\"");
        if (end_pos == std::string::npos) {
            s.clear();
            return;
        }
        s.erase(end_pos + 1);
    }
};

#endif // CSV_LOADER_HPP
