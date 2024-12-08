
#include <iostream>
#include <random>
#include <utility>

#include "micrograd.hpp"
#include "dataframe.hpp"

using namespace microgradCpp;

/*

g++ -g -std=c++17 -Iinclude -O2 -o main main_df.cpp
*/

int main()
{
    namespaceCpp::DataFrame df;

    try
    {
        df.from_csv("data/iris.csv", true, ',');

        df.print();
        df.encode_column("variety");
        // df.print();
        df.print_shape();
        df.rocking_star_print();

        df.to_csv("test1.csv");
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
