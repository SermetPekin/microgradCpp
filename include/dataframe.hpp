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
#include <iomanip>
#include <numeric>
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

#include <fstream>
#include <iostream>
#include <optional>
#include <typeindex>
#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <variant>
#include "series.hpp"
#include "header.hpp"
#include "range.hpp"
#include "console_utils.hpp"

namespace microgradCpp

{
    class DataFrame;
    inline void save_as_csv(const DataFrame &df, const std::string &filename, std::optional<char> delimiter = std::nullopt);

    using Cell = std::variant<std::monostate, double, long long, std::string>;

    using Column = std::vector<Cell>;

    class DataFrame
    {
    public:
        void from_csv(const std::string &filename, bool has_header = true, char delimiter = ',');

        std::unordered_map<std::string, Column> columns;
        std::unordered_map<std::string, std::optional<std::type_index>> column_types;
        std::unordered_map<std::string, std::unordered_map<std::string, int>> encoding_mappings;

        std::vector<std::string> column_order;

        static inline bool DEFAULT_INPLACE = true;

        DataFrame operator()(const std::initializer_list<int> &row_indices, const std::vector<std::string> &col_names)
        {
            return this->slice(std::vector<size_t>(row_indices.begin(), row_indices.end()), col_names, DEFAULT_INPLACE);
        }

        DataFrame operator()(const std::initializer_list<int> &row_indices)
        {
            return this->slice(std::vector<size_t>(row_indices.begin(), row_indices.end()), column_order, DEFAULT_INPLACE);
        }

        DataFrame operator()(const Range &range, const std::vector<std::string> &col_names)
        {

            auto numbers = range.to_vector<size_t>();

            return this->slice(numbers, col_names, DEFAULT_INPLACE);
        }

        DataFrame operator()(const Range &range)
        {

            auto numbers = range.to_vector<size_t>();

            return this->slice(numbers, column_order, DEFAULT_INPLACE);
        }

        DataFrame operator()(const std::vector<size_t> &row_indices, const std::vector<std::string> &col_names)
        {
            return this->slice(get_all_row_indices(), column_order, DEFAULT_INPLACE);
        }

        DataFrame operator()()
        {

            return this->slice(get_all_row_indices(), column_order, DEFAULT_INPLACE);
        }

        DataFrame operator()(const std::vector<size_t> &row_indices)
        {
            return this->slice(row_indices, column_order, DEFAULT_INPLACE);
        }

        DataFrame operator()(const std::vector<std::string> &col_names)
        {

            return this->slice(get_all_row_indices(), col_names, DEFAULT_INPLACE);
        }

        DataFrame cols(const std::vector<std::string> &col_names, bool inplace = DEFAULT_INPLACE)
        {

            return this->slice(get_all_row_indices(), col_names, inplace);
        }
         

        DataFrame slice(const std::vector<size_t> &row_indices, const std::vector<std::string> &col_names, bool inplace = DEFAULT_INPLACE)
        {

            size_t num_rows = columns.empty() ? 0 : columns.begin()->second.size();
            for (size_t row_idx : row_indices)
            {
                if (row_idx >= num_rows)
                {

                    epic_out_of_range("Row index " + std::to_string(row_idx) + " is out of bounds. DataFrame has " + std::to_string(num_rows) + " rows.");
                }
            }

            std::unordered_map<std::string, Column> new_columns;
            std::unordered_map<std::string, std::optional<std::type_index>> new_column_types;
            std::unordered_map<std::string, std::unordered_map<std::string, int>> new_encoding_mappings;

            for (const auto &col_name : col_names)
            {
                if (columns.find(col_name) == columns.end())
                {
                    throw std::invalid_argument("Column " + col_name + " not found");
                }

                Column new_col;
                for (const auto &row_idx : row_indices)
                {
                    if (row_idx >= columns.at(col_name).size())
                    {
                        throw std::out_of_range("Row index out of range");
                    }
                    new_col.push_back(columns.at(col_name)[row_idx]);
                }

                new_columns[col_name] = std::move(new_col);
                new_column_types[col_name] = column_types.at(col_name);

                if (encoding_mappings.find(col_name) != encoding_mappings.end())
                {
                    new_encoding_mappings[col_name] = encoding_mappings.at(col_name);
                }
            }

            if (inplace)
            {
                columns = std::move(new_columns);
                column_types = std::move(new_column_types);
                encoding_mappings = std::move(new_encoding_mappings);
                column_order = col_names;
                return *this;
            }
            else
            {
                DataFrame result;
                result.columns = std::move(new_columns);
                result.column_types = std::move(new_column_types);
                result.encoding_mappings = std::move(new_encoding_mappings);
                result.column_order = col_names;
                return result;
            }
        }

        DataFrame()
        {
        }
        DataFrame copy() const
        {
            DataFrame new_df;
            new_df.columns = columns;
            new_df.column_types = column_types;
            new_df.encoding_mappings = encoding_mappings;
            new_df.column_order = column_order;
            return new_df;
        }

        // ............................................................. get_column_names
        std::vector<std::string> get_column_names() const
        {

            return column_order;
            // std::vector<std::string> names;
            // for (const auto &[name, _] : columns)
            // {
            //     names.push_back(name);
            // }
            // return names;
        }

        void print_shape() const
        {
            size_t num_rows = 0;
            if (!columns.empty())
            {
                num_rows = columns.begin()->second.size();
            }
            size_t num_columns = columns.size();

            std::cout << "📊 DataFrame Shape: [" << num_rows << " rows x " << num_columns << " columns]\n";
        }

        // ............................................................. get_variant_type_index
        std::type_index get_variant_type_index(const Cell &value)
        {
            return std::visit([](auto &&arg) -> std::type_index
                              {
        using T = std::decay_t<decltype(arg)>;
        return std::type_index(typeid(T)); }, value);
        }

        // ............................................................. add_value
        void add_value(const std::string &column_name, const Cell &value)
        {
            if (columns.find(column_name) == columns.end())
            {
                columns[column_name] = Column();

                column_types[column_name] = get_variant_type_index(value);
            }

            columns[column_name].push_back(value);
        }

        void add_value_at(const std::string &column_name, size_t position, const Cell &value)
        {
            if (columns.find(column_name) == columns.end())
            {
                columns[column_name] = Column();
                column_types[column_name] = get_variant_type_index(value);
            }

            Column &column = columns[column_name];
            if (column.size() <= position)
            {
                column.resize(position + 1, std::monostate{});
            }

            column[position] = value;
        }

        std::type_index get_column_type(const std::string &column_name) const
        {
            auto it = column_types.find(column_name);
            if (it != column_types.end() && it->second.has_value())
            {
                return it->second.value();
            }
            else
            {
                return typeid(void);
            }
        }

        template <typename T>
        std::optional<std::vector<T>> values(const std::string &column_name) const
        {
            auto it = columns.find(column_name);
            if (it != columns.end())
            {
                const Column &col = it->second;
                std::vector<T> result;
                result.reserve(col.size());

                for (const auto &cell : col)
                {
                    if (std::holds_alternative<T>(cell))
                    {
                        result.push_back(std::get<T>(cell));
                    }
                    else
                    {
                        // missing or incompatible types
                        if constexpr (std::is_arithmetic_v<T>)
                        {
                            result.push_back(std::numeric_limits<T>::quiet_NaN());
                        }
                        else
                        {
                            result.push_back(T{});
                        }
                    }
                }
                return result;
            }
            return std::nullopt;
        }

        std::string clean_colname(const std::string &col) const
        {
            std::string str = col;
            std::replace(str.begin(), str.end(), '.', '_');
            return str;
        }

        Series operator[](const std::string &column_name) const
        {
            std::string c_colname = clean_colname(column_name);

            auto it = columns.find(c_colname);
            if (it == columns.end())
            {
                throw std::invalid_argument("Column not found: " + c_colname);
            }
            return Series(it->second);
        }

        template <typename T>
        void to_csv(const T &file_name, std::optional<char> delimiter = std::nullopt)
        {
            if constexpr (std::is_same_v<T, std::string> ||
                          std::is_same_v<T, const char *> ||
                          (std::is_array_v<T> && std::is_same_v<std::remove_extent_t<T>, char>))
            {
                m_save_csv(file_name, delimiter);
            }
            else
            {
                static_assert(std::is_same_v<T, std::string> ||
                                  std::is_same_v<T, const char *> ||
                                  (std::is_array_v<T> && std::is_same_v<std::remove_extent_t<T>, char>),
                              "Unsupported file name type. Must be std::string, const char*, or char array.");
            }
        }

        void show()
        {
            auto cols = get_column_names();

            std::cout << divider();
            prints("Columns : \n");
            std::cout << divider();
            print_v(cols);
        }

        void print() const
        {
            rocking_star_print();
        }
        void printOld() const
        {
            for (const auto &[column_name, data] : columns)
            {
                std::cout << column_name << ": \n";
                std::cout << divider();

                for (const auto &cell : data)
                {
                    std::visit([](const auto &v)
                               {
                        using T = std::decay_t<decltype(v)>;
                        if constexpr (std::is_same_v<T, std::monostate>)
                        {
                            std::cout << "NaN" << " ";
                        }
                        else
                        {
                            std::cout << v << " ";
                        } }, cell);
                }

                std::cout << "\n"
                          << microgradCpp::divider() << "\n";
            }
        }

        void encode_column(const std::string &column_name)
        {
            auto it = columns.find(column_name);
            if (it == columns.end())
            {
                throw std::invalid_argument("Column not found: " + column_name);
            }

            Column &column = it->second;
            std::unordered_map<std::string, double> encoding_map;
            double index = 0.0;

            // Create the encoding map
            for (const auto &cell : column)
            {
                if (std::holds_alternative<std::string>(cell))
                {
                    const std::string &value = std::get<std::string>(cell);
                    if (encoding_map.find(value) == encoding_map.end())
                    {
                        encoding_map[value] = index++;
                    }
                }
            }

            // Replace the column values with their encoded doubles
            for (auto &cell : column)
            {
                if (std::holds_alternative<std::string>(cell))
                {
                    const std::string &value = std::get<std::string>(cell);
                    cell = encoding_map[value];
                }
            }

            // Update the column type to double
            column_types[column_name] = typeid(double);
        }

        void print_encoding_map(const std::string &column_name) const
        {
            auto it = encoding_mappings.find(column_name);
            if (it == encoding_mappings.end())
            {
                std::cout << "No encoding map found for column: " << column_name << "\n";
                return;
            }

            std::cout << "Encoding map for column '" << column_name << "':\n";
            for (const auto &[key, value] : it->second)
            {
                std::cout << key << " -> " << value << "\n";
            }
        }

        std::string get_type_string(const std::string &column_name) const
        {
            auto it = column_types.find(column_name);
            if (it != column_types.end() && it->second.has_value())
            {
                std::type_index type = it->second.value();
                if (type == typeid(double))
                    return "double";
                if (type == typeid(long long))
                    return "int";
                if (type == typeid(std::string))
                    return "string";
            }
            return "unknown";
        }

        void rocking_star_print(size_t n = 10) const
        {
            std::cout << "\n🚀 DataFrame Overview 🚀\n";
            std::cout << "========================\n";

            // Display shape
            size_t num_rows = 0;
            if (!columns.empty())
            {
                num_rows = columns.begin()->second.size();
            }
            std::cout << "📝 Shape: (" << num_rows << " rows, " << columns.size() << " columns)\n";

            // Display column names and types
            std::cout << "\n🧩 Columns and Data Types:\n";
            std::cout << "---------------------------\n";
            for (const auto &name : column_order)
            {
                std::cout << "🔹 " << std::setw(15) << std::left << name << " | [" << get_type_string(name) << "]\n";
            }

            // Display first 'n' rows
            std::cout << "\n🔍 First " << n << " Rows:\n";
            std::cout << "---------------------------\n";

            // Print column headers in the correct order
            for (const auto &name : column_order)
            {
                std::cout << std::setw(15) << std::left << name;
            }
            std::cout << "\n";

            // Print separator line
            for (size_t i = 0; i < column_order.size(); ++i)
            {
                std::cout << std::setw(15) << std::setfill('-') << "" << std::setfill(' ');
            }
            std::cout << "\n";

            // Print rows in the correct order
            for (size_t row = 0; row < std::min(n, num_rows); ++row)
            {
                for (const auto &name : column_order)
                {
                    const auto &col = columns.at(name);
                    if (row < col.size())
                    {
                        std::visit([](const auto &value)
                                   {
                    using T = std::decay_t<decltype(value)>;
                    if constexpr (std::is_same_v<T, std::monostate>) {
                        std::cout << std::setw(15) << "NaN";
                    } else {
                        std::cout << std::setw(15) << value;
                    } }, col[row]);
                    }
                    else
                    {
                        std::cout << std::setw(15) << "NaN";
                    }
                }
                std::cout << "\n";
            }

            std::cout << "========================\n\n";
        }

        // Rocking start print method
        // void rocking_star_printBackup(size_t n = 10) const
        // {
        //     std::cout << "\n🚀 DataFrame Overview 🚀\n";
        //     std::cout << "========================\n";

        //     // Display shape
        //     size_t num_rows = 0;
        //     if (!columns.empty())
        //     {
        //         num_rows = columns.begin()->second.size();
        //     }
        //     std::cout << "📝 Shape: (" << num_rows << " rows, " << columns.size() << " columns)\n";

        //     // Display column names and types
        //     std::cout << "\n🧩 Columns and Data Types:\n";
        //     std::cout << "---------------------------\n";
        //     for (const auto &[name, col] : columns)
        //     {
        //         std::cout << "🔹 " << std::setw(15) << std::left << name << " | [" << get_type_string(name) << "]\n";
        //     }

        //     // Display first 'n' rows
        //     std::cout << "\n🔍 First " << n << " Rows:\n";
        //     std::cout << "---------------------------\n";

        //     // Print column headers
        //     for (const auto &[name, _] : columns)
        //     {
        //         std::cout << std::setw(15) << std::left << name;
        //     }
        //     std::cout << "\n";

        //     // Print separator line
        //     for (size_t i = 0; i < columns.size(); ++i)
        //     {
        //         std::cout << std::setw(15) << std::setfill('-') << "" << std::setfill(' ');
        //     }
        //     std::cout << "\n";

        //     // Print rows
        //     for (size_t row = 0; row < std::min(n, num_rows); ++row)
        //     {
        //         for (const auto &[_, col] : columns)
        //         {
        //             if (row < col.size())
        //             {
        //                 std::visit([](const auto &value)
        //                            {
        //                     using T = std::decay_t<decltype(value)>;
        //                     if constexpr (std::is_same_v<T, std::monostate>) {
        //                         std::cout << std::setw(15) << "NaN";
        //                     } else {
        //                         std::cout << std::setw(15) << value;
        //                     } }, col[row]);
        //             }
        //             else
        //             {
        //                 std::cout << std::setw(15) << "NaN";
        //             }
        //         }
        //         std::cout << "\n";
        //     }

        //     std::cout << "========================\n\n";
        // }

        // void add_column(const std::string &name, const Column &col)
        // {
        //     columns[name] = col;
        //     column_order.push_back(name);
        // }

        void add_column(const std::string &name, const Column &col, std::optional<std::type_index> type = std::nullopt)
        {
            columns[name] = col;
            column_order.push_back(name);
            column_types[name] = type;
        }

    private:
        std::vector<size_t> get_all_row_indices() const
        {
            size_t num_rows = columns.empty() ? 0 : columns.begin()->second.size();
            std::vector<size_t> indices(num_rows);
            std::iota(indices.begin(), indices.end(), 0);
            return indices;
        }

        void m_save_csv(const std::string &file_name, std::optional<char> delimiter = std::nullopt)
        {
            save_as_csv(*this, file_name, delimiter);
        }

        void m_save_csv(const char *file_name, std::optional<char> delimiter = std::nullopt)
        {
            const std::string file_name_(file_name);
            save_as_csv(*this, file_name_, delimiter);
        }
    }; // class

    //     inline void save_as_csv(const DataFrame &df, const std::string &filename, std::optional<char> delimiter = std::nullopt);

} // namespace