/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#ifndef EXC_HPP
#define EXC_HPP
#include <exception>
#include <iostream>
class NotImplementedException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Function not implemented yet.";
    }
};
#endif //EXC_HPP