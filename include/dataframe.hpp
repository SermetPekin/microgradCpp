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

namespace evds
{

    class DataFrame;
 
    void save_as_csv(const DataFrame &df, const std::string &filename, std::optional<char> delimiter = std::nullopt) ; 
    using Cell = std::variant<std::monostate, double, long long, std::string>;

    using Column = std::vector<Cell>;

    class DataFrame
    {
    public:
        std::unordered_map<std::string, Column> columns;
        std::unordered_map<std::string, std::optional<std::type_index>> column_types;

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
        void to_csv(const T &file_name , std::optional<char> delimiter = std::nullopt )
        {
            if constexpr (std::is_same_v<T, std::string> ||
                          std::is_same_v<T, const char *> ||
                          (std::is_array_v<T> && std::is_same_v<std::remove_extent_t<T>, char>))
            {
                m_save_csv(file_name , delimiter );
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
                          << divider() << "\n";
            }
        }

    private:
        void m_save_csv(const std::string &file_name ,  std::optional<char> delimiter = std::nullopt )
        {
            save_as_csv(*this, file_name , delimiter );
        }

        void m_save_csv(const char *file_name ,  std::optional<char> delimiter = std::nullopt )
        {
            const std::string file_name_(file_name );
            save_as_csv(*this, file_name_ , delimiter );
        }

  
    };
 

void save_as_csv(const DataFrame &df, const std::string &filename, std::optional<char> delimiter  )
{   
    static std::string NaNstr("") ;
    std::cout << "[saving csv] " << filename << "\n";
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    char actual_delimiter = delimiter.value_or(';');

    auto column_names = df.get_column_names();
    size_t max_num_rows = 0;

    for (const auto &col_name : column_names) {
        auto col_data = df.columns.at(col_name);
        max_num_rows = std::max(max_num_rows, col_data.size());
    }

    // Write headers
    for (size_t col = 0; col < column_names.size(); ++col) {
        file << column_names[col];
        if (col < column_names.size() - 1)
            file << actual_delimiter;
    }
    file << "\n";

    // Write data
    for (size_t row = 0; row < max_num_rows; ++row) {
        for (size_t col = 0; col < column_names.size(); ++col) {
            const auto &col_name = column_names[col];
            const auto &col_data = df.columns.at(col_name);

            if (row < col_data.size()) {
                const auto &cell = col_data[row];

                std::visit([&](const auto &value) {
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
                    }
                }, cell);
            } else {
                file << NaNstr ;
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