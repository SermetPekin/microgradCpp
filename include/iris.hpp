
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


#ifndef IRIS_LOADER_HPP
#define IRIS_LOADER_HPP

#include "value.hpp"
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

class IrisLoader {
public:
    static void load_iris(const std::string& url,
                          std::vector<std::vector<std::shared_ptr<Value>>>& inputs,
                          std::vector<std::vector<std::shared_ptr<Value>>>& targets) {
        
        std::unordered_map<std::string, std::vector<double>> species_map = {
            {"Setosa", {1.0, 0.0, 0.0}},
            {"Versicolor", {0.0, 1.0, 0.0}},
            {"Virginica", {0.0, 0.0, 1.0}}
        };

        std::ifstream file(url);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open Iris dataset: " + url);
        }

        std::string line;

        // Attempt to read the first line and check if it's a header
        if (std::getline(file, line)) {
            if (line.find("sepal.length") != std::string::npos) {
                std::cout << "Skipping header row: " << line << std::endl;
            } else {
                // Not a header, rewind the file to process this line as data
                file.clear();
                file.seekg(0, std::ios::beg);
            }
        }

        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;  
            }

            std::stringstream ss(line);
            std::string value;

            // Parse the four features
            std::vector<std::shared_ptr<Value>> row;
            for (int i = 0; i < 4; ++i) {
                if (!std::getline(ss, value, ',')) {
                    throw std::runtime_error("Unexpected end of line reading features from: " + line);
                }

                trim(value);
                double val;
                try {
                    val = std::stod(value);
                } catch (...) {
                    throw std::runtime_error("Error parsing feature as double: '" + value + "' in line: " + line);
                }
                row.push_back(std::make_shared<Value>(val));
            }
            inputs.push_back(row);

           
            if (!std::getline(ss, value, ',')) {
                throw std::runtime_error("No species column found in line: " + line);
            }

            trim(value);

            auto species_it = species_map.find(value);
            if (species_it == species_map.end()) {
                throw std::runtime_error("Unknown species: '" + value + "'");
            }

            std::vector<std::shared_ptr<Value>> target;
            for (auto v : species_it->second) {
                target.push_back(std::make_shared<Value>(v));
            }
            targets.push_back(target);
        }

        file.close();

        if (inputs.empty()) {
            throw std::runtime_error("No data loaded from Iris dataset. Check file format or path.");
        }

        std::cout << "Loaded " << inputs.size() << " samples from " << url << std::endl;
    }

private:
    // Helper function to trim whitespace and quotes
    static void trim(std::string &s) {
        // Remove leading whitespace and quotes
        size_t start_pos = s.find_first_not_of(" \t\"");
        if (start_pos == std::string::npos) {
            s.clear();
            return;
        }
        s.erase(0, start_pos);

        // Remove trailing whitespace and quotes
        size_t end_pos = s.find_last_not_of(" \t\"");
        if (end_pos == std::string::npos) {
            s.clear();
            return;
        }
        s.erase(end_pos + 1);
    }
};

#endif // IRIS_LOADER_HPP
