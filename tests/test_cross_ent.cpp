/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/

#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include "value.hpp"
#include "loss.hpp"
#include "micrograd.hpp"
#include "types.hpp"
using namespace microgradCpp;

inline std::shared_ptr<Value> make_value(double data)
{
    return std::make_shared<Value>(data);
}

TEST(LossTest, CrossEntropyLoss)
{
    std::vector<std::shared_ptr<Value>> predictions = {
        make_value(0.7),  
        make_value(0.2),  
        make_value(0.1)   
    };

    std::vector<std::shared_ptr<Value>> targets = {
        make_value(0.0),  
        make_value(1.0),  
        make_value(0.0)   
    };

    auto loss = Loss::cross_entropy(predictions, targets);

    double expected_loss = -std::log(0.2);

    ASSERT_NEAR(loss->data, expected_loss, 1e-4);
}
