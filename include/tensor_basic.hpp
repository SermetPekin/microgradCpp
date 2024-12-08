#include <vector>
#include <iostream>

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

