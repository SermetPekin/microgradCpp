#pragma once

#include <iostream>
#include <functional>
#include <stdexcept>

std::function<void()> _EXCEPTION_out_of_bound = []() {
    throw std::out_of_range("Index out of bounds");
};