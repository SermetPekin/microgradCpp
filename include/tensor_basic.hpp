
#include <vector>
#include <iostream>
/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
class Tensor {
public:
    std::vector<std::vector<double>> data;
    Tensor(const std::vector<std::vector<double>>& data) : data(data) {}
    void print() const {
        for (const auto& row : data) {
            for (double val : row) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
    }
};