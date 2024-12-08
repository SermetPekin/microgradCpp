#ifndef SCHEMA_HPP
#define SCHEMA_HPP
// MIT License

// Copyright (c) [2024] Sermet Pekin

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// */

#include <vector>
#include <string>
#include <unordered_map>

class Schema {
public:
    std::vector<std::string> columns;
    std::vector<std::string> types;  // e.g., "double", "string", "category"

    Schema(const std::vector<std::string>& columns, const std::vector<std::string>& types)
        : columns(columns), types(types) {}

    size_t get_column_index(const std::string& name) const {
        for (size_t i = 0; i < columns.size(); ++i) {
            if (columns[i] == name) return i;
        }
        throw std::runtime_error("Column not found: " + name);
    }
};


#endif // SCHEMA_HPP