
#pragma once
/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#include <iostream>
#include <functional>
#include <stdexcept>
inline std::function<void()> _EXCEPTION_out_of_bound = []()
{
    throw std::out_of_range("Index out of bounds");
};