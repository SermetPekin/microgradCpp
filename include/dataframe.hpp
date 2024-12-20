
/*
 * evdscpp: An open-source data wrapper for accessing the EVDS API.
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
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
#include <typeindex>
#include <iomanip>
#include <algorithm>
#include "series.hpp"
#include "header.hpp"
#include "range.hpp"
#include "console_utils.hpp"
#include "types.hpp"
#include <algorithm> // For std::shuffle
#include <random>    // For std::default_random_engine
#include <chrono>    // For seeding the random engine
namespace microgradCpp
{
    class DataFrame;
    inline void save_as_csv(const DataFrame &df, const std::string &filename, std::optional<char> delimiter = std::nullopt);
    using Cell = std::variant<std::monostate, double, long long, std::string>;
    using Column = std::vector<Cell>;
    class DataFrame
    {
        /*
        *
        *
        *   @usage :
            DataFrame df;
            df.from_csv("./data/wine.csv", true, ';');
            df.normalize();
            df.normalize(Range(2,5)); // columns between 2 and 4
            df.normalize(Range(5)); //   first 5 columns {0,1,2,3,4 }
            df.encode_column("quality"); // one hot encoding for column name 'quality'
            df.encode_column( columns ); // one hot encoding for column names
            df.encode_column(Range(5 ) ); // one hot encoding for  first 5 columns
            df.print();
            df.shuffle();
            df.print();
        */
    public:
        void from_csv(const std::string &filename, bool has_header = true, char delimiter = ',');
        std::unordered_map<std::string, Column> columns;
        std::unordered_map<std::string, std::optional<std::type_index>> column_types;
        std::unordered_map<std::string, std::unordered_map<std::string, int>> encoding_mappings;
        std::vector<std::string> column_order;
        static inline bool DEFAULT_INPLACE = true;
        int size() const
        {
            return get_all_row_indices().size();
        }
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
        DataFrame operator()(const Range &rows_, const Range &cols_)
        {
            auto numbers = rows_.to_vector<size_t>();
            v_string cols_filtered = cols_.filter(column_order);
            return this->slice(numbers, cols_filtered, DEFAULT_INPLACE);
        }
        DataFrame rows(const Range &range)
        {
            auto numbers = range.to_vector<size_t>();
            return this->slice(numbers, column_order, DEFAULT_INPLACE);
        }
        v_string v(const Range &column_range)
        {
            v_string items;
            for (size_t i = 0; i < column_order.size(); ++i)
            {
                if (column_range.includes(i))
                {
                    items.push_back(column_order[i]);
                }
            }
            return items;
        }
        vv_double to_vv_double() const
        {
            vv_double result;
            if (columns.empty())
                return result;
            // Determine the number of rows based on the first column
            size_t num_rows = columns.begin()->second.size();
            // Iterate through each row
            for (size_t i = 0; i < num_rows; ++i)
            {
                std::vector<double> row;
                for (const auto &col_name : column_order)
                {
                    const auto &col = columns.at(col_name);
                    if (i < col.size())
                    {
                        const auto &cell = col[i];
                        if (std::holds_alternative<double>(cell))
                        {
                            row.push_back(std::get<double>(cell));
                        }
                        else
                        {
                            row.push_back(0.0);
                        }
                    }
                }
                result.push_back(row);
            }
            return result;
        }
        // vv_string v(const Range &colum_range){
        //     vv_string items ;
        //     for(int i =0 ; i< column_order.size() ; i++ ){
        //             if( colum_range.includes( i ))
        //                 items.push_back( column_order[ i ]) ;
        //     }
        //     return items ;
        // }
        DataFrame subset(const Range &range, const Range &colum_range)
        {
            auto numbers = range.to_vector<size_t>();
            return this->slice(numbers, column_order, DEFAULT_INPLACE);
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
        // DataFrame operator()(const Range &range)
        // {
        //     return this->slice(range.to_vector<size_t>(), column_order, DEFAULT_INPLACE);
        // }
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
        void shuffle()
        {
            if (columns.empty())
            {
                throw std::runtime_error("Cannot shuffle an empty DataFrame.");
            }
            size_t num_rows = columns.begin()->second.size();
            //   a vector of indices representing row positions
            std::vector<size_t> indices(num_rows);
            std::iota(indices.begin(), indices.end(), 0);
            //   the random number generator
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(indices.begin(), indices.end(), std::default_random_engine(seed));
            //   new columns with shuffled rows
            std::unordered_map<std::string, Column> shuffled_columns;
            for (const auto &col_name : column_order)
            {
                Column &col = columns[col_name];
                Column shuffled_col;
                for (size_t i : indices)
                {
                    shuffled_col.push_back(col[i]);
                }
                shuffled_columns[col_name] = shuffled_col;
            }
            // Replace the original columns with shuffled columns
            columns = shuffled_columns;
        }
        void normalize(const v_string &cols_given = {})
        {
            v_string cols_to_normalize;
            if (cols_given.empty())
                cols_to_normalize = column_order;
            else
                cols_to_normalize = cols_given;
            normalize_internal(cols_to_normalize);
        }
        void normalize(const Range &range)
        {
            v_string some_cols = range.filter(column_order);
            return normalize_internal(some_cols);
        }
        void normalize_internal(const v_string &cols_given)
        {
            for (const auto &col_name : cols_given)
            {
                auto &col = columns.at(col_name);
                // Step 1: Collect numeric values
                std::vector<double> numeric_values;
                for (auto &cell : col)
                {
                    if (std::holds_alternative<double>(cell))
                    {
                        numeric_values.push_back(std::get<double>(cell));
                    }
                    else if (std::holds_alternative<long long>(cell))
                    {
                        numeric_values.push_back(static_cast<double>(std::get<long long>(cell)));
                    }
                }
                if (numeric_values.empty())
                {
                    std::cout << "Skipping non-numeric column: " << col_name << std::endl;
                    continue; // Skip non-numeric columns
                }
                // Step 2: Calculate mean and standard deviation
                double mean = std::accumulate(numeric_values.begin(), numeric_values.end(), 0.0) / numeric_values.size();
                double std_dev = std::sqrt(std::accumulate(numeric_values.begin(), numeric_values.end(), 0.0,
                                                           [mean](double acc, double val)
                                                           { return acc + (val - mean) * (val - mean); }) /
                                           numeric_values.size());
                if (std_dev == 0)
                {
                    std::cout << "Standard deviation is zero; skipping normalization for column: " << col_name << std::endl;
                    continue;
                }
                // Step 3: Normalize numeric cells in the column
                for (auto &cell : col)
                {
                    if (std::holds_alternative<double>(cell))
                    {
                        cell = (std::get<double>(cell) - mean) / std_dev;
                    }
                    else if (std::holds_alternative<long long>(cell))
                    {
                        cell = (static_cast<double>(std::get<long long>(cell)) - mean) / std_dev;
                    }
                }
                std::cout << "Normalized column: " << col_name << std::endl;
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
        void encode_column(const Range &range)
        {
            v_string columns_ = range.filter(column_order);
            for (const string x : columns_)
                encode_column(x);
        }
        void encode_column(const v_string &columns_)
        {
            for (const string x : columns_)
                encode_column(x);
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