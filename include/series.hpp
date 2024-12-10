
/*
 * evdscpp: An open-source data wrapper for accessing the EVDS API.
 * Author: Sermet Pekin
 *
 
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#pragma once
#include "header.hpp"
#include <variant>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <limits> // For NaN
#include <optional>
#include <numeric>
#include <cmath>
namespace microgradCpp
{
    using Cell = std::variant<std::monostate, double, long long, std::string>;
    class Series
    {
    public:
        Series(std::vector<Cell> data) : data_(std::move(data)) {}
        // .................................................................. size
        size_t size() const
        {
            return data_.size();
        }
        void normalize()
        {
            std::vector<double> numeric_values;
            // Step 1: Collect numeric values
            for (const auto &cell : data_)
            {
                std::visit([&](auto &&arg)
                           {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, double> || std::is_same_v<T, long long>) {
                    numeric_values.push_back(static_cast<double>(arg));
                } }, cell);
            }
            if (numeric_values.empty())
            {
                std::cerr << "No numeric values to normalize." << std::endl;
                return;
            }
            // Step 2: Calculate mean and standard deviation
            double mean = std::accumulate(numeric_values.begin(), numeric_values.end(), 0.0) / numeric_values.size();
            double std_dev = std::sqrt(std::accumulate(numeric_values.begin(), numeric_values.end(), 0.0,
                                                       [mean](double acc, double val)
                                                       { return acc + (val - mean) * (val - mean); }) /
                                       numeric_values.size());
            if (std_dev == 0)
            {
                std::cerr << "Standard deviation is zero; cannot normalize." << std::endl;
                return;
            }
            // Step 3: Normalize numeric cells in data_
            for (auto &cell : data_)
            {
                std::visit([&](auto &arg)
                           {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, double> || std::is_same_v<T, long long>) {
                    arg = (static_cast<double>(arg) - mean) / std_dev;
                } }, cell);
            }
        }
        // .................................................................. values_internal
        template <typename T>
        std::vector<T> values_internal() const
        {
            std::vector<T> result;
            result.reserve(data_.size());
            for (const auto &cell : data_)
            {
                result.push_back(std::visit([](const auto &value) -> T
                                            { return convert<T>(value); }, cell));
            }
            return result;
        }
        // .................................................................. values
        std::vector<double> values() const
        {
            return values_internal<double>();
        }
        // .................................................................. at
        template <typename T>
        T at(size_t index) const
        {
            if (index >= data_.size())
            {
                throw std::out_of_range("Index out of range");
            }
            return std::visit([](const auto &value) -> T
                              { return convert<T>(value); }, data_[index]);
        }
        // .................................................................. print
        void print() const
        {
            for (const auto &cell : data_)
            {
                std::visit([](const auto &value)
                           {
                    using T = std::decay_t<decltype(value)>;
                    if constexpr (std::is_same_v<T, std::monostate>)
                    {
                        std::cout << "NaN" << " ";
                    }
                    else
                    {
                        std::cout << value << " ";
                    } }, cell);
            }
            std::cout << std::endl;
        }
    private:
        std::vector<Cell> data_;
        // .................................................................. convert
        template <typename T, typename U>
        static T convert(const U &value)
        {
            if constexpr (std::is_same_v<T, U>)
            {
                return value;
            }
            else if constexpr (std::is_same_v<U, std::monostate>)
            {
                if constexpr (std::is_same_v<T, double>)
                {
                    return std::numeric_limits<double>::quiet_NaN();
                }
                else if constexpr (std::is_same_v<T, std::optional<long long>>)
                {
                    return std::nullopt;
                }
                else if constexpr (std::is_same_v<T, std::string>)
                {
                    return "NaN";
                }
                else if constexpr (std::is_same_v<T, std::monostate>)
                {
                    return "NaN";
                }
                else
                {
                    throw std::invalid_argument("Unsupported conversion from monostate");
                }
            }
            else if constexpr (std::is_arithmetic_v<U> && std::is_arithmetic_v<T>)
            {
                return static_cast<T>(value);
            }
            else if constexpr (std::is_same_v<U, std::string> && std::is_arithmetic_v<T>)
            {
                std::istringstream iss(value);
                T result;
                iss >> result;
                if (iss.fail())
                {
                    throw std::invalid_argument("Conversion from string to numeric failed");
                }
                return result;
            }
            else if constexpr (std::is_arithmetic_v<U> && std::is_same_v<T, std::string>)
            {
                return std::to_string(value);
            }
            else
            {
                throw std::invalid_argument("Unsupported conversion");
            }
        }
    };
}