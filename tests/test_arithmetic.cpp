/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#include "value.hpp"
#include <iostream>
#include <iostream>
#include <cassert>
#include <random>

void test_arithmetic_operations2() {
    auto a = std::make_shared<Value>(2.0, "a");
    auto b = std::make_shared<Value>(3.0, "b");

    auto c = a + b;  
    auto d = a * b;  
    auto e = d / b;  
    auto f = e - a;  

    
    assert(c->data == 5.0 && "Addition failed!");
    assert(d->data == 6.0 && "Multiplication failed!");
    assert(e->data == 2.0 && "Division failed!");
    assert(f->data == 0.0 && "Subtraction failed!");

  
    assert(c->parents.size() == 2 && "Addition parents incorrect!");
    assert(d->parents.size() == 2 && "Multiplication parents incorrect!");
    assert(e->parents.size() == 2 && "Division parents incorrect!");
    assert(f->parents.size() == 2 && "Subtraction parents incorrect!");

    std::cout << "Arithmetic operations test passed!" << std::endl;
}


void test_forward_log_exp2() {
    auto a = std::make_shared<Value>(2.71828, "a");

    auto b = a->log();  
    auto c = b->exp();  

    //   results
    assert(std::abs(b->data - 1.0) < 1e-5 && "Logarithm failed!");
    assert(std::abs(c->data - 2.71828) < 1e-5 && "Exponentiation failed!");

    //   parents
    assert(b->parents.size() == 1 && "Logarithm parents incorrect!");
    assert(c->parents.size() == 1 && "Exponentiation parents incorrect!");

    std::cout << "Forward log and exp test passed!" << std::endl;
}

void test_backpropagation2() {
    auto a = std::make_shared<Value>(2.0, "a");
    auto b = std::make_shared<Value>(3.0, "b");

    auto c = a + b;        
    auto d = c * b;        
    auto loss = d->log();  

    loss->backward();

   
    double d_grad = 1 / d->data; // dL/dd
    double c_grad = b->data * d_grad; // dL/dc = b * dL/dd
    double b_grad = c->data * d_grad + d_grad; // dL/db = c * dL/dd + dL/db (via c)

    assert(std::abs(loss->grad - 1.0) < 1e-5 && "Loss gradient incorrect!");
    assert(std::abs(d->grad - d_grad) < 1e-5 && "Gradient for d incorrect!");
    assert(std::abs(c->grad - c_grad) < 1e-5 && "Gradient for c incorrect!");
    assert(std::abs(b->grad - b_grad) < 1e-5 && "Gradient for b incorrect!");
    assert(std::abs(a->grad - d_grad) < 1e-5 && "Gradient for a incorrect!");

    std::cout << "Backpropagation test passed!" << std::endl;
}


void test_simple_back2(){
auto a = std::make_shared<Value>(2.0, "a");
auto b = std::make_shared<Value>(3.0, "b");
auto c = a * b;          // c=6
auto loss = c->log();    // log(6)
loss->backward();

}
 

