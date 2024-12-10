/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/

#include <gtest/gtest.h>
#include <memory>
#include "value.hpp"
#include "sgd.hpp"  

class OptimizerTest : public ::testing::Test {
protected:
    std::shared_ptr<Value> param1;
    std::shared_ptr<Value> param2;
    std::vector<std::shared_ptr<Value>> params;

    void SetUp() override {
        param1 = std::make_shared<Value>(1.0);
        param2 = std::make_shared<Value>(-2.0);
        params = {param1, param2};
    }
};

TEST_F(OptimizerTest, SGDUpdatesParameters) {
    param1->grad = 0.5;
    param2->grad = -0.3;

    SGD sgd(0.1, 0.9);

    double initial_param1 = param1->data;
    double initial_param2 = param2->data;

    sgd.step(params);

    EXPECT_NE(param1->data, initial_param1);
    EXPECT_NE(param2->data, initial_param2);

    EXPECT_DOUBLE_EQ(param1->data, 1.0 - 0.1 * 0.5);
    EXPECT_DOUBLE_EQ(param2->data, -2.0 - 0.1 * (-0.3));
}

TEST_F(OptimizerTest, NesterovSGDUpdatesParameters) {
    param1->grad = 0.5;
    param2->grad = -0.3;

    NesterovSGD nesterov(0.1, 0.9);

    double initial_param1 = param1->data;
    double initial_param2 = param2->data;

    nesterov.step(params);

    EXPECT_NE(param1->data, initial_param1);
    EXPECT_NE(param2->data, initial_param2);
}

TEST_F(OptimizerTest, ZeroGradResetsGradients) {
    param1->grad = 0.5;
    param2->grad = -0.3;

    SGD sgd(0.1);

    sgd.zero_grad(params);

    EXPECT_DOUBLE_EQ(param1->grad, 0.0);
    EXPECT_DOUBLE_EQ(param2->grad, 0.0);
}

