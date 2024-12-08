#include <iostream>
#include <unordered_map>
#include <vector>
#include <cassert>

// Function definition
std::unordered_map<std::string, double> create_target_encoding(const std::vector<std::string>& target_column) {
    std::unordered_map<std::string, double> encoding_map;
    double index = 0.0;

    for (const auto& value : target_column) {
        if (encoding_map.find(value) == encoding_map.end()) {
            encoding_map[value] = index++;
        }
    }

    return encoding_map;
}

// Test function
void test_create_target_encoding() {
    std::vector<std::string> targets = {"setosa", "setosa", "setosa", "versicolor", "virginica", "setosa", "virginica"};

    auto encoding = create_target_encoding(targets);

    // Expected: {"setosa" -> 0.0, "versicolor" -> 1.0, "virginica" -> 2.0}
    assert(encoding["setosa"] == 0.0);
    assert(encoding["versicolor"] == 1.0);
    assert(encoding["virginica"] == 2.0);

    std::cout << "test_create_target_encoding passed!" << std::endl;
}

int main() {
    test_create_target_encoding();
    return 0;
}


//g++ -std=c++17 -o test_encoding t_encoding.cpp

/* 
g++ -std=c++17 -o test_encoding t_encoding.cpp
*/