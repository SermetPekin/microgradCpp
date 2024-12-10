
/*
 * evdscpp: An open-source data wrapper for accessing the EVDS API.
 * Author: Sermet Pekin
 *
 *
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include <sstream>
#include <algorithm>
#include <utility>
namespace microgradCpp
{
    template <typename T>
    void print_v(const T &v)
    {
        for (const auto &x : v)
            std::cout << x << ", ";
        std::cout << std::endl;
    }
    template <typename T>
    void prints(const T &x)
    {
        std::cout << x;
    }
    inline bool confirm(const std::string &message, const std::string &url, bool verbose)
    {
        std::string input;
        if (verbose)
            std::cout << "[" << url << "]\n";
        std::cout << message << " (y/n): ";
        std::cin >> input;
        return input == "y" || input == "Y";
    }
    inline std::string read_content_from_file(const std::string &filename)
    {
        std::ifstream infile(filename, std::ios::in);
        if (!infile)
        {
            std::cerr << "Failed to open file for reading: " << filename << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << infile.rdbuf();
        infile.close();
        std::cout << "Content read from " << filename << std::endl;
        return buffer.str();
    }
    inline std::string getEvds_test()
    {
        std::string resp_file = "response.json";
        auto resp = read_content_from_file(resp_file);
        return resp;
    }
    inline std::string divider()
    {
        return "\n================================\n";
    }
}