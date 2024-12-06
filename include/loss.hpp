#ifndef LOSS_HPP
#define LOSS_HPP

#include "value.hpp"
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

class Loss {
public:
    static std::shared_ptr<Value> cross_entropy(
        const std::vector<std::shared_ptr<Value>>& predictions,
        const std::vector<std::shared_ptr<Value>>& targets
    ) {
        // Assumes:
        // 1. predictions are already probabilities (from softmax in MLP forward)
        // 2. targets are one-hot encoded: exactly one element is 1, others are 0
        // cross entropy = -sum_i t_i * log(p_i)

        auto loss = std::make_shared<Value>(0.0);

        for (size_t i = 0; i < predictions.size(); ++i) {
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
