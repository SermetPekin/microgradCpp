#ifndef COLUMNAR_DATA_HPP
#define COLUMNAR_DATA_HPP
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



#include <vector>
#include <string>
#include <variant>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <memory>

#include "types.hpp"

using namespace microgradCpp;

class Column
{
public:
    std::string name; // Name of the column
    ColumnData data;  // Data stored in the column (double or string)

    // Constructor
    Column(const std::string &column_name, const ColumnData &column_data)
        : name(column_name), data(column_data) {}

    size_t size() const
    {
        return std::visit([](const auto &col)
                          { return col.size(); }, data);
    }

    void print() const
    {
        std::cout << "Column: " << name << "\n";
        std::visit([](const auto &col)
                   {
            for (const auto& val : col) {
                std::cout << val << " ";
            }
            std::cout << "\n"; }, data);
    }
};

class Table
{
private:
    std::vector<std::shared_ptr<Column>> columns;

public:
    // Add a new column
    void add_column(const std::string &name, const ColumnData &data)
    {
        columns.push_back(std::make_shared<Column>(name, data));
    }

    // Get a column by name
    std::shared_ptr<Column> get_column(const std::string &name) const
    {
        for (const auto &col : columns)
        {
            if (col->name == name)
            {
                return col;
            }
        }
        throw std::runtime_error("Column not found: " + name);
    }

    bool column_exists(const std::string &column_name) const
    {

        for (const auto &col : columns)
        {
            if (col->name == column_name)
            {
                return true;
            }
        }
        return false;
    }

    // Print the entire table (for debugging)
    void
    print() const
    {
        for (const auto &col : columns)
        {
            col->print();
        }
    }

    // Get row data (combine all columns for a specific row index)
    std::vector<std::string> get_row(size_t index) const
    {
        std::vector<std::string> row;
        for (const auto &col : columns)
        {
            std::visit([&row, index](const auto &col_data)
                       {
            if (index < col_data.size()) {
                if constexpr (std::is_same_v<std::decay_t<decltype(col_data[0])>, std::string>) {
                    row.push_back(col_data[index]); // No conversion needed for std::string
                } else {
                    row.push_back(std::to_string(col_data[index])); // Convert numeric types to string
                }
            } else {
                throw std::runtime_error("Row index out of bounds");
            } }, col->data);
        }
        return row;
    }

    // Get number of rows
    size_t num_rows() const
    {
        if (columns.empty())
            return 0;
        return columns[0]->size();
    }
};

#endif // COLUMNAR_DATA_HPP
