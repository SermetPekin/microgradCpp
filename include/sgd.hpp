
#ifndef OPTIMIZER_HPP
#define OPTIMIZER_HPP
/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
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