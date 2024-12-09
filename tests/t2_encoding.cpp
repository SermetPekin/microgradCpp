// #include <iostream>
// #include <vector>
// #include <string>
// #include <cassert>

// using vv_string = std::vector<std::vector<std::string>>;
// using vv_double = std::vector<std::vector<double>>;
// inline  
// std::unordered_map<std::string, double> create_target_encoding(const std::vector<std::string>& target_column) {
//     std::unordered_map<std::string, double> encoding_map;
//     double index = 0.0;

//     for (const auto& value : target_column) {
//         if (encoding_map.find(value) == encoding_map.end()) {
//             encoding_map[value] = index++;
//         }
//     }

//     return encoding_map;
// }



 
// vv_double convert_to_double_with_encoding(const vv_string& data, bool has_header = false) {
//     vv_double converted_data;
//     size_t start_index = has_header ? 1 : 0;

//      std::vector<std::string> target_column;
//     for (size_t i = start_index; i < data.size(); ++i) {
//         target_column.push_back(data[i].back());
//     }

    
//     auto target_encoding_map = create_target_encoding(target_column);

//     for (size_t i = start_index; i < data.size(); ++i) {
//         std::vector<double> converted_row;

//         // Convert all but the last column (features) to double
//         for (size_t j = 0; j < data[i].size() - 1; ++j) {
//             try {
//                 converted_row.push_back(std::stod(data[i][j]));
//             } catch (const std::invalid_argument&) {
//                 converted_row.push_back(0.0);  // Fallback for invalid numeric conversion
//             }
//         }

//         // Encode the target column using the encoding map
//         converted_row.push_back(target_encoding_map[data[i].back()]);
//         converted_data.push_back(converted_row); 
//     }

//     return converted_data;
// }

// // Helper function to print the dataset
// void print_dataset(const vv_double& data) {
//     for (const auto& row : data) {
//         for (const auto& val : row) {
//             std::cout << val << " ";
//         }
//         std::cout << "\n";
//     }
// }

// // Test function
// void test_convert_to_double_with_encoding() {
//     vv_string data = {
//         {"5.1", "3.5", "1.4", "0.2", "setosa"},
//         {"7.0", "3.2", "4.7", "1.4", "versicolor"},
//         {"6.3", "3.3", "6.0", "2.5", "virginica"},
//         {"5.0", "3.6", "1.4", "0.2", "setosa"}
//     };

//     vv_double converted_data = convert_to_double_with_encoding(data);

//     // Print converted data for verification
//     print_dataset(converted_data);

//     // Check the number of rows and columns
//     assert(converted_data.size() == 4);
//     assert(converted_data[0].size() == 5);

//     // Check the target encodings
//     double target1 = converted_data[0].back();
//     double target2 = converted_data[1].back();
//     double target3 = converted_data[2].back();
//     double target4 = converted_data[3].back();

//     assert(target1 == target4); // Both "setosa"
//     assert(target2 != target1); // "versicolor" should have a different encoding
//     assert(target3 != target1 && target3 != target2); // "virginica" should have a different encoding

//     std::cout << "test_convert_to_double_with_encoding passed!" << std::endl;
// }

// int main() {
//     test_convert_to_double_with_encoding();
//     return 0;
// }

// /*

// g++ -std=c++17 -o test_encoding t2_encoding.cpp
// */
