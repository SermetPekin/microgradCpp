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

namespace namespaceCpp
{

    class DataFrame;

    void save_as_csv(const DataFrame &df, const std::string &filename, std::optional<char> delimiter = std::nullopt);
    using Cell = std::variant<std::monostate, double, long long, std::string>;

    using Column = std::vector<Cell>;

    class DataFrame
    {
    public:
        void from_csv(const std::string &filename, bool has_header, char delimiter);

        std::unordered_map<std::string, Column> columns;
        std::unordered_map<std::string, std::optional<std::type_index>> column_types;
        std::unordered_map<std::string, std::unordered_map<std::string, int>> encoding_mappings;

        // ............................................................. get_column_names
        std::vector<std::string> get_column_names() const
        {
            std::vector<std::string> names;
            for (const auto &[name, _] : columns)
            {
                names.push_back(name);
            }
            return names;
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

            // for (const auto &[column_name, data] : columns)
            // {
            //     std::cout << column_name << ": \n";
            //     std::cout << divider();

            //     for (const auto &cell : data)
            //     {
            //         std::visit([](const auto &v)
            //                    {
            //             using T = std::decay_t<decltype(v)>;
            //             if constexpr (std::is_same_v<T, std::monostate>)
            //             {
            //                 std::cout << "NaN" << " ";
            //             }
            //             else
            //             {
            //                 std::cout << v << " ";
            //             } }, cell);
            //     }

            //     std::cout << "\n"
            //               << divider() << "\n";
            // }
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

        // // Encode a column's string values to numerical indices
        // void encode_column(const std::string &column_name)
        // {
        //     if (columns.find(column_name) == columns.end())
        //     {
        //         throw std::invalid_argument("Column not found: " + column_name);
        //     }

        //     Column &col = columns[column_name];

        //     // Create a set of unique string values
        //     std::set<std::string> unique_values;
        //     for (const auto &cell : col)
        //     {
        //         if (std::holds_alternative<std::string>(cell))
        //         {
        //             unique_values.insert(std::get<std::string>(cell));
        //         }
        //     }

        //     // Create a mapping from string to numerical indices
        //     std::unordered_map<std::string, int> encoding_map;
        //     int index = 0;
        //     for (const auto &value : unique_values)
        //     {
        //         encoding_map[value] = index++;
        //     }

        //     // Replace the strings with their encoded values
        //     for (auto &cell : col)
        //     {
        //         if (std::holds_alternative<std::string>(cell))
        //         {
        //             cell = static_cast<double>(encoding_map[std::get<std::string>(cell)]);
        //         }
        //     }

        //     // Store the encoding map for reference
        //     encoding_mappings[column_name] = encoding_map;

        //     std::cout << "Column '" << column_name << "' encoded successfully.\n";
        // }

        // Print the encoding map for a given column
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

        // Function to get the type as a string
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

        // Rocking start print method
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
            for (const auto &[name, col] : columns)
            {
                std::cout << "🔹 " << std::setw(15) << std::left << name << " | [" << get_type_string(name) << "]\n";
            }

            // Display first 'n' rows
            std::cout << "\n🔍 First " << n << " Rows:\n";
            std::cout << "---------------------------\n";

            // Print column headers
            for (const auto &[name, _] : columns)
            {
                std::cout << std::setw(15) << std::left << name;
            }
            std::cout << "\n";

            // Print separator line
            for (size_t i = 0; i < columns.size(); ++i)
            {
                std::cout << std::setw(15) << std::setfill('-') << "" << std::setfill(' ');
            }
            std::cout << "\n";

            // Print rows
            for (size_t row = 0; row < std::min(n, num_rows); ++row)
            {
                for (const auto &[_, col] : columns)
                {
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

    private:
        void m_save_csv(const std::string &file_name, std::optional<char> delimiter = std::nullopt)
        {
            save_as_csv(*this, file_name, delimiter);
        }

        void m_save_csv(const char *file_name, std::optional<char> delimiter = std::nullopt)
        {
            const std::string file_name_(file_name);
            save_as_csv(*this, file_name_, delimiter);
        }
    };

    void save_as_csv(const DataFrame &df, const std::string &filename, std::optional<char> delimiter)
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

} // namespace