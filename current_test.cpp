
#include "micrograd.hpp"
#include "value.hpp"
#include "mlp.hpp"
using namespace microgradCpp;
/*
    --file_name ./data/iris.csv
    --encode variety
*/
int main(int argc, char *argv[])
{
    // DatasetType dataset = get_iris();
    DataFrame df;
    df.from_csv("./data/wine.csv", true, ';');
    // df.normalize(  );
    df.encode_column("quality");
    df.print();
    df.shuffle();
    df.print();
    double TRAIN_SIZE{0.8};
    // Create MLP model
    // Input: 4 features, hidden layers: [7,7], output: 3 classes
    // Define the model and hyperparameters
    // MLP model(4, {10, 10, 3});
    MLP model(4, {16, 16, 10});
    auto params = model.parameters();
    double learning_rate = 0.01;
    int epochs; //  = 100;
    std::cout << "Epoch : ?";
    std::cin >> epochs;
    AdamOptimizer optimizer(params, learning_rate);
    train_eval(df, TRAIN_SIZE, model, optimizer, epochs);
    return 0;
}