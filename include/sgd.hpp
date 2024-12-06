#ifndef OPTIMIZER_HPP
#define OPTIMIZER_HPP

#include "value.hpp"
#include <vector>
#include <memory>

class SGD {
public:
    double lr; // Learning rate
    double momentum;

//    explicit SGD(double learning_rate) : lr(learning_rate) {}
    explicit SGD(double learning_rate, double momentum_factor = 0.0)
        : lr(learning_rate), momentum(momentum_factor) {}

void step(const std::vector<std::shared_ptr<Value>>& parameters) {
    for (auto& param : parameters) {
        if (param->grad != 0.0) { // Skip if gradient is zero
            param->data -= lr * param->grad; // Gradient descent step
        }
    }
}




     
    void zero_grad(const std::vector<std::shared_ptr<Value>>& parameters) {
        for (auto& param : parameters) {
            param->grad = 0.0; // Reset gradients to zero
        }
    }
};

#endif // OPTIMIZER_HPP
