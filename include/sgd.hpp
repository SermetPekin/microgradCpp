#ifndef OPTIMIZER_HPP
#define OPTIMIZER_HPP
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

#include "value.hpp"
#include <vector>
#include <memory>
#include "value.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

class BaseOptimizer
{
public:
    double lr;
    double momentum;
    std::unordered_map<Value *, double> velocity;

    explicit BaseOptimizer(double learning_rate, double momentum_factor = 0.0)
        : lr(learning_rate), momentum(momentum_factor) {}

    virtual void step(const std::vector<std::shared_ptr<Value>> &parameters) = 0;

    void zero_grad(const std::vector<std::shared_ptr<Value>> &parameters)
    {
        for (auto &param : parameters)
        {
            param->grad = 0.0;
        }
    }
};

class SGD : public BaseOptimizer
{
public:
    using BaseOptimizer::BaseOptimizer;

    void step(const std::vector<std::shared_ptr<Value>> &parameters) override
    {
        for (auto &param : parameters)
        {
            if (param->grad != 0.0)
            {
                if (velocity.find(param.get()) == velocity.end())
                {
                    velocity[param.get()] = 0.0;
                }

                velocity[param.get()] = momentum * velocity[param.get()] - lr * param->grad;
                param->data += velocity[param.get()];
            }
        }
    }
};

class NesterovSGD : public BaseOptimizer
{
public:
    using BaseOptimizer::BaseOptimizer;

    void step(const std::vector<std::shared_ptr<Value>> &parameters) override
    {
        for (auto &param : parameters)
        {
            if (param->grad != 0.0)
            {
                if (velocity.find(param.get()) == velocity.end())
                {
                    velocity[param.get()] = 0.0;
                }

                double prev_velocity = velocity[param.get()];
                velocity[param.get()] = momentum * velocity[param.get()] - lr * param->grad;
                param->data += -momentum * prev_velocity + (1 + momentum) * velocity[param.get()];
            }
        }
    }
};

 

#endif // OPTIMIZER_HPP
