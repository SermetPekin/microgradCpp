
#ifndef TYPES_HPP
#define TYPES_HPP
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
#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <unordered_map>
#include <memory>
#include "value.hpp"
namespace microgradCpp
{
    using string = std::string;
    using v_string = std::vector<std::string>;
    using vv_string = std::vector<std::vector<std::string>>;

  
    using v_double = std::vector<double>;
    using vv_double = std::vector<std::vector<double>>;

    using vv_shared_Value = std::vector<std::vector<std::shared_ptr<Value>>>;
    using v_shared_Value = std::vector<std::shared_ptr<Value>>;
    using shared_Value = std::shared_ptr<Value>;

    using ColumnData = std::variant<std::vector<double>, std::vector<std::string>>;
    using DatasetType = std::vector<std::pair<std::vector<std::shared_ptr<Value>>, std::vector<std::shared_ptr<Value>>>>;
    using ColRows = std::vector<std::vector<std::shared_ptr<Value>>>;

    // microgradCpp namespace

}
#endif // TYPES_HPP