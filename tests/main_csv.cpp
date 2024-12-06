#include "./include/csv.hpp"
#include "./include/value.hpp"
#include <iostream>
#include <vector>
#include <string>

int main() {
    try {
        // Example: Load a CSV file, skipping the first line if it's a header
        auto data = CSVLoader::load_csv("./data/iris.csv", true);

 
        for (const auto& row : data) {
            for (const auto& cell : row) {
                std::cout << cell << " ";
            }
            std::cout << "\n";
        }

 

    } catch (const std::exception& e) {
        std::cerr << "Error loading CSV: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
