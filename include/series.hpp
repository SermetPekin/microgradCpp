
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
#include "header.hpp"
#include <variant>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <limits> // For NaN
#include <optional>

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