
#ifndef LOSS_HPP
#define LOSS_HPP
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
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>
#include "value.hpp"
#include "console_utils.hpp"
#include "data_utils.hpp"
using namespace microgradCpp;
inline void display_predictions_and_targets(const std::vector<std::shared_ptr<Value>> &predictions,
                                            const std::vector<std::shared_ptr<Value>> &targets)
{
    const int width = 15; // Width for each column
    std::cout << "\nÄŸÅ¸â€œÅ  Predictions vs Targets\n";
    std::cout << "=============================================\n";
    std::cout << std::setw(width) << "Predictions" << std::setw(width) << "Targets" << "\n";
    std::cout << "---------------------------------------------\n";
    size_t max_size = std::max(predictions.size(), targets.size());
    for (size_t i = 0; i < max_size; ++i)
    {
        // Display prediction value or placeholder if out of bounds
        if (i < predictions.size() && predictions[i])
        {
            std::cout << std::setw(width) << predictions[i]->data;
        }
        else
        {
            std::cout << std::setw(width) << "N/A";
        }
        // Display target value or placeholder if out of bounds
        if (i < targets.size() && targets[i])
        {
            std::cout << std::setw(width) << targets[i]->data;
        }
        else
        {
            std::cout << std::setw(width) << "N/A";
        }
        std::cout << "\n";
    }
    std::cout << "=============================================\n";
}
class Loss
{
public:
    static std::shared_ptr<Value> cross_entropy(
        const std::vector<std::shared_ptr<Value>> &predictions,
        const std::vector<std::shared_ptr<Value>> &targets)
    {
        // Assumes:
        // 1. predictions are already probabilities (from softmax in MLP forward)
        // 2. targets are one-hot encoded: exactly one element is 1, others are 0
        // cross entropy = -sum_i t_i * log(p_i)
        // display_predictions_and_targets(predictions, targets);
        // stop();
        if (predictions.size() != targets.size() || !(predictions.size() > 0))
        {
            std::cout << predictions.size() << " predictions <== ==> targets " << targets.size();
            stop("problem");
        }
        auto loss = std::make_shared<Value>(0.0);
        for (size_t i = 0; i < predictions.size(); ++i)
        {
            // log(p_i)
            auto logp = predictions[i]->log();
            // accumulate t_i * log(p_i)
            loss = loss + (targets[i] * logp);
        }
        // final loss = - (sum t_i * log(p_i))
        // The negative sign creates a new node, so backprop will work through it
        return -loss;
    }
};
#endif // LOSS_HPP