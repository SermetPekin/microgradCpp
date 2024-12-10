/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
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
