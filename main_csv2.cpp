

#include "./include/csv.hpp"
#include "./include/value.hpp"
#include "./include/schema.hpp"
// #include "./include/dataprocessor.hpp"
#include "./include/micrograd.hpp"
#include <iostream>
#include <vector>
#include <string>
/*
g++ -std=c++17 -Iinclude -O2 -o main main_csv2.cpp


*/

template <class T>
void print(const T &data, int MAX_VAL = 5)
{

    int say = 0;
    for (const auto &row : data)
    {
        say++;
        if (say > MAX_VAL)
            break;
        for (double value : row)
        {

            std::cout << value << " ";
        }
        std::cout << "\n";
    }
}
int main()
{

    DataProcessor processor;

    // Example dataset with mixed types (Iris-like data)
    std::vector<std::vector<std::string>> data = {
        {"5.1", "3.5", "1.4", ".2", "Iris-setosa"},
        {"7.0", "3.2", "4.7", ".4", "Iris-versicolor"},
        {"6.3", "3.3", "6.0", ".5", "Iris-virginica"},
        {"5.0", "3.6", "1.4", ".2", "Iris-setosa"}};

    vv_double converted_data = processor.convert_to_double_with_encoding_(data);

    vv_string data2 = processor.load_and_process("./data/iris.csv", true);

    vv_double converted_data2 = processor.convert_to_double_with_encoding_(data2);

    print(converted_data2);

    DatasetType dataset1 = get_iris();
    DatasetType dataset2 = get_iris2();
    //  DatasetType dataset3 = get_iris2();

    write_dataset_to_csv(dataset1, "get_iris.csv");
    write_dataset_to_csv(dataset2, "get_iris2.csv"); // ok
                                              // write_dataset_to_csv( dataset2, "d2.csv") ;  // ok

    write_to_csv(data, "output.csv");
    write_to_csv(data2, "data2.csv");
    write_to_csv(converted_data, "converted_data.csv");
    write_to_csv(converted_data2, "converted_data2.csv");


  

}