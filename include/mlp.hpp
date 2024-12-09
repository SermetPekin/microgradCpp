
#ifndef MLP_HPP
#define MLP_HPP
/*
MIT License
Copyright (c) [2024] Sermet Pekin
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "linear.hpp"
#include <vector>
#include <memory>
#include <cmath>
#include <random>
#include <iostream>
inline std::vector<std::shared_ptr<Value>> leaky_relu(const std::vector<std::shared_ptr<Value>> &inputs)
{
    std::vector<std::shared_ptr<Value>> outputs;
    for (const auto &val : inputs)
    {
        if (val->data > 0)
        {
            outputs.push_back(val);
        }
        else
        {
            outputs.push_back(std::make_shared<Value>(0.01 * val->data)); // Small negative slope
        }
    }
    return outputs;
}
// ReLU Act. Func.
inline std::vector<std::shared_ptr<Value>> relu(const std::vector<std::shared_ptr<Value>> &inputs)
{
    std::vector<std::shared_ptr<Value>> outputs;
    for (auto &input : inputs)
    {
        double val = (input->data > 0) ? input->data : 0.0;
        auto out = std::make_shared<Value>(val);
        out->add_parent(input);
        out->_backward = [input, out]()
        {
            // d/dx ReLU(x)=1 if x>0 else 0
            if (input->data > 0)
            {
                input->grad += out->grad;
            }
            else
            {
                input->grad += 0;
            }
        };
        outputs.push_back(out);
    }
    return outputs;
}
// Dropout
inline std::vector<std::shared_ptr<Value>> dropout(const std::vector<std::shared_ptr<Value>> &inputs, double rate, bool training)
{
    if (!training)
    {
        return inputs;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution drop_dist(1.0 - rate);
    std::vector<std::shared_ptr<Value>> outputs;
    for (auto &input : inputs)
    {
        bool keep = drop_dist(gen);
        double val = keep ? input->data : 0.0;
        auto out = std::make_shared<Value>(val);
        out->add_parent(input);
        out->_backward = [input, out, keep]()
        {
            // If kept, gradient flows as is; if dropped, gradient=0.
            if (keep)
            {
                input->grad += out->grad;
            }
            else
            {
                input->grad += 0;
            }
        };
        outputs.push_back(out);
    }
    return outputs;
}
// Softmax Activation Function
inline std::vector<std::shared_ptr<Value>> softmax(const std::vector<std::shared_ptr<Value>> &inputs)
{
    // sum_exp = sum of exp(input)
    auto sum_exp = std::make_shared<Value>(0.0);
    for (auto &inp : inputs)
    {
        auto e = inp->exp(); // e = exp(inp)
        sum_exp = sum_exp + e;
    }
    // each output = exp(input) / sum_exp
    std::vector<std::shared_ptr<Value>> outputs;
    for (auto &inp : inputs)
    {
        auto e = inp->exp();
        auto prob = e / sum_exp;
        outputs.push_back(prob);
    }
    return outputs;
}
class MLP
{ /* TODO options object */
    int _in_features;
    std::vector<int> _layer_sizes;

public:
    std::vector<std::shared_ptr<Linear>> layers;
    int input_size() const
    {
        return _in_features; //  4 , {  10,10, 3 } => 4
    }
    int output_size() const
    {
        return _layer_sizes.back(); //  4 , {  10,10, 3 } => 3
    }
    std::vector<int> get_layer_sizes() const
    {
        return _layer_sizes;
    }
    MLP(int in_features, const std::vector<int> &layer_sizes)
    {
        _in_features = in_features;
        _layer_sizes = layer_sizes;
        int current_in = in_features;
        for (int size : layer_sizes)
        {
            layers.push_back(std::make_shared<Linear>(current_in, size));
            current_in = size;
        }
    }
    std::vector<std::shared_ptr<Value>> forward(const std::vector<std::shared_ptr<Value>> &inputs, bool training = true)
    {
        if (inputs.empty())
        {
            throw std::runtime_error("Empty input provided to MLP forward pass.");
        }
        auto activations = inputs;
        for (size_t i = 0; i < layers.size(); ++i)
        {
            activations = layers[i]->forward(activations);
            // For hidden layers, apply ReLU and Dropout
            if (i < layers.size() - 1)
            {
                // activations = relu(activations);
                activations = leaky_relu(activations);
                // Removed batch_norm due to complexity.
                activations = dropout(activations, 0.20, training);
            }
        }
        // Apply softmax to the final layer's output
        return softmax(activations);
    }
    std::vector<std::shared_ptr<Value>> parameters() const
    {
        std::vector<std::shared_ptr<Value>> params;
        for (const auto &layer : layers)
        {
            auto layer_params = layer->parameters();
            params.insert(params.end(), layer_params.begin(), layer_params.end());
        }
        return params;
    }
};
#endif // MLP_HPP