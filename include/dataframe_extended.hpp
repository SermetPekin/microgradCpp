
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
 *
 *
 *
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
#include "dataframe.hpp"
namespace microgradCpp
{
    class DataFrameExtended : public DataFrame
    {
    public:
        struct testMyStruct
        {
            int x, y;
        };
        testMyStruct abcdefg{5, 6};
    };
}