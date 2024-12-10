
/*
 * evdscpp: An open-source data wrapper for accessing the EVDS API.
 * Author: Sermet Pekin
 *
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
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