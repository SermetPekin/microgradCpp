
#include "micrograd.hpp"
#include "value.hpp"
#include "mlp.hpp"
using namespace microgradCpp;
int main()
{
    // DatasetType dataset = get_iris();
    DataFrame df;
    df.from_csv("./data/iris.csv");
    df.normalize();
    df.encode_column("variety");
    df.print();
    df.shuffle();
    df.print();
    return 0;
}