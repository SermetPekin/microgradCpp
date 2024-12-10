
#ifndef TYPES_HPP
#define TYPES_HPP
/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <unordered_map>
#include <memory>
#include "value.hpp"
namespace microgradCpp
{
    using string = std::string;
    using v_string = std::vector<std::string>;
    using vv_string = std::vector<std::vector<std::string>>;
    using v_double = std::vector<double>;
    using vv_double = std::vector<std::vector<double>>;
    using vv_shared_Value = std::vector<std::vector<std::shared_ptr<Value>>>;
    using v_shared_Value = std::vector<std::shared_ptr<Value>>;
    using shared_Value = std::shared_ptr<Value>;
    using ColumnData = std::variant<std::vector<double>, std::vector<std::string>>;
    using DatasetType = std::vector<std::pair<std::vector<std::shared_ptr<Value>>, std::vector<std::shared_ptr<Value>>>>;
    using ColRows = std::vector<std::vector<std::shared_ptr<Value>>>;
    // microgradCpp namespace
}
#endif // TYPES_HPP