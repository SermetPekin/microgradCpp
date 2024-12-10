
/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
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
namespace microgradCpp
{
    static inline std::string trim(const std::string &str)
    {
        auto start = str.find_first_not_of(" \t");
        auto end = str.find_last_not_of(" \t");
        return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
    }
    inline bool is_numeric(const std::string &str)
    {
        std::string trimmed = trim(str);
        if (trimmed.empty())
            return false;
        char *end = nullptr;
        std::strtod(trimmed.c_str(), &end);
        return end == trimmed.c_str() + trimmed.size();
    }
    // // Implementation of the slicing operator()
    // DataFrame DataFrame::operator()(const std::vector<size_t> &row_indices, const std::vector<std::string> &col_names)
    // {
    //     DataFrame result;
    //     for (const auto &col_name : col_names)
    //     {
    //         if (columns.find(col_name) == columns.end())
    //         {
    //             throw std::invalid_argument("Column " + col_name + " not found");
    //         }
    //         Column new_col;
    //         for (const auto &row_idx : row_indices)
    //         {
    //             if (row_idx >= columns.at(col_name).size())
    //             {
    //                 throw std::out_of_range("Row index out of range");
    //             }
    //             new_col.push_back(columns.at(col_name)[row_idx]);
    //         }
    //         result.add_column(col_name, new_col);
    //     }
    //     return result;
    // }
    inline void save_as_csv(const DataFrame &df, const std::string &filename, std::optional<char> delimiter)
    {
        static std::string NaNstr("");
        std::cout << "[saving csv] " << filename << "\n";
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        char actual_delimiter = delimiter.value_or(',');
        auto column_names = df.get_column_names();
        size_t max_num_rows = 0;
        for (const auto &col_name : column_names)
        {
            auto col_data = df.columns.at(col_name);
            max_num_rows = std::max(max_num_rows, col_data.size());
        }
        // Write headers
        for (size_t col = 0; col < column_names.size(); ++col)
        {
            file << column_names[col];
            if (col < column_names.size() - 1)
                file << actual_delimiter;
        }
        file << "\n";
        // Write data
        for (size_t row = 0; row < max_num_rows; ++row)
        {
            for (size_t col = 0; col < column_names.size(); ++col)
            {
                const auto &col_name = column_names[col];
                const auto &col_data = df.columns.at(col_name);
                if (row < col_data.size())
                {
                    const auto &cell = col_data[row];
                    std::visit([&](const auto &value)
                               {
                    using T = std::decay_t<decltype(value)>;
                    if constexpr (std::is_same_v<T, std::monostate>) {
                        file << NaNstr ;
                    } else if constexpr (std::is_same_v<T, double>) {
                        if (std::isnan(value))
                            file << NaNstr ;
                        else
                            file << value;
                    } else {
                        file << value;
                    } }, cell);
                }
                else
                {
                    file << NaNstr;
                }
                if (col < column_names.size() - 1)
                    file << actual_delimiter;
            }
            file << "\n";
        }
        file.close();
        std::cout << "CSV file saved as: " << filename << std::endl;
    }
    inline void DataFrame::from_csv(const std::string &filename, bool has_header, char delimiter) // = true, ','
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Error opening file: " + filename);
        }
        std::string line;
        // std::vector<std::string> column_names;
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
                column_order = cells;
                for (auto &col : column_order)
                {
                    col = trim(col); // TODO
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
                        std::string col_name = "column_" + std::to_string(i);
                        column_order.push_back(col_name);
                        columns[col_name] = Column();
                        column_types[col_name] = std::nullopt;
                        // column_names.push_back("column_" + std::to_string(i));
                        // columns[column_names[i]] = Column();
                        // column_types[column_names[i]] = std::nullopt;
                    }
                    is_first_line = false;
                }
                for (size_t i = 0; i < cells.size(); ++i)
                {
                    const auto &col_name = column_order[i];
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
    // namespace
}