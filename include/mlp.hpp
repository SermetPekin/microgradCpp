#ifndef MLP_HPP
#define MLP_HPP

#include "linear.hpp"
#include <vector>
#include <memory>
#include <cmath>
#include <random>
#include <iostream>

// ReLU Activation Function
// Creates new nodes so gradient flows correctly.
inline
std::vector<std::shared_ptr<Value>> relu(const std::vector<std::shared_ptr<Value>>& inputs) {
    std::vector<std::shared_ptr<Value>> outputs;
    for (auto& input : inputs) {
        double val = (input->data > 0) ? input->data : 0.0;
        auto out = std::make_shared<Value>(val);
        out->add_parent(input);
        out->_backward = [input, out]() {
            // d/dx ReLU(x)=1 if x>0 else 0
            if (input->data > 0) {
                input->grad += out->grad;
            } else {
                input->grad += 0;
            }
        };
        outputs.push_back(out);
    }
    return outputs;
}

// Dropout
// Creates nodes so gradient flows. If dropped, gradient=0; if kept, gradient passes through.
inline 
std::vector<std::shared_ptr<Value>> dropout(const std::vector<std::shared_ptr<Value>>& inputs, double rate, bool training) {
    if (!training) {
        // No dropout during evaluation, just return inputs directly.
        return inputs;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution drop_dist(1.0 - rate);

    std::vector<std::shared_ptr<Value>> outputs;
    for (auto &input : inputs) {
        bool keep = drop_dist(gen);
        double val = keep ? input->data : 0.0;
        auto out = std::make_shared<Value>(val);
        out->add_parent(input);
        out->_backward = [input, out, keep]() {
            // If kept, gradient flows as is; if dropped, gradient=0.
            if (keep) {
                input->grad += out->grad;
            } else {
                input->grad += 0;
            }
        };
        outputs.push_back(out);
    }
    return outputs;
}

// Softmax Activation Function
// Implemented using exp and arithmetic on Value nodes for proper gradient tracking.
inline
std::vector<std::shared_ptr<Value>> softmax(const std::vector<std::shared_ptr<Value>>& inputs) {
    // sum_exp = sum of exp(input)
    auto sum_exp = std::make_shared<Value>(0.0);
    for (auto &inp : inputs) {
        auto e = inp->exp(); // e = exp(inp)
        sum_exp = sum_exp + e;
    }

    // each output = exp(input) / sum_exp
    std::vector<std::shared_ptr<Value>> outputs;
    for (auto &inp : inputs) {
        auto e = inp->exp();
        auto prob = e / sum_exp;
        outputs.push_back(prob);
    }
    return outputs;
}

// MLP class
class MLP {
public:
    std::vector<std::shared_ptr<Linear>> layers;

    // Constructor: Create an MLP with specified layer sizes
    MLP(int in_features, const std::vector<int>& layer_sizes) {
        int current_in = in_features;
        for (int size : layer_sizes) {
            layers.push_back(std::make_shared<Linear>(current_in, size));
            current_in = size;
        }
    }

    // Forward pass
    std::vector<std::shared_ptr<Value>> forward(const std::vector<std::shared_ptr<Value>>& inputs, bool training = true) {
        auto activations = inputs;
        for (size_t i = 0; i < layers.size(); ++i) {
            activations = layers[i]->forward(activations);
            // For hidden layers, apply ReLU and Dropout
            if (i < layers.size() - 1) {
                activations = relu(activations);
                // Removed batch_norm due to complexity.
                activations = dropout(activations, 0.05, training);
            }
        }
        // Apply softmax to the final layer's output
        return softmax(activations);
    }

    // Get all parameters (weights and biases)
    std::vector<std::shared_ptr<Value>> parameters() const {
        std::vector<std::shared_ptr<Value>> params;
        for (const auto& layer : layers) {
            auto layer_params = layer->parameters();
            params.insert(params.end(), layer_params.begin(), layer_params.end());
        }
        return params;
    }
};

#endif // MLP_HPP




 