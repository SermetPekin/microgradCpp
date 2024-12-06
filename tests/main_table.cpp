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

#include "./include/column.hpp"
#include <iostream>
#include <vector>

int main() {
    Table table;

    // Add a numeric column
    table.add_column("Feature1", std::vector<double>{1.0, 2.0, 3.0});
    table.add_column("Feature2", std::vector<double>{4.0, 5.0, 6.0});

    // Add a categorical column
    table.add_column("Label", std::vector<std::string>{"Setosa", "Versicolor", "Virginica"});

    // Print the table
    table.print();

    // Access a specific column
    auto feature1 = table.get_column("Feature1");
    feature1->print();

    // Get a specific row
    auto row = table.get_row(1);
    std::cout << "Row 1: ";
    for (const auto& val : row) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    return 0;
}
