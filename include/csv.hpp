
/*
 * MIT License
 *
 * Copyright (c) 2024 Sermet Pekin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files...
*/
#ifndef CSV_L_HPP
#define CSV_L_HPP
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "types.hpp"
using namespace microgradCpp ;
class CSVLoader
{
public:
    /**
     * @brief Load a CSV file into a 2D vector of strings.
     *
     * This function does not parse data types. It only splits rows by a delimiter and returns cells as strings.
     * If skip_header is true, it will skip the first non-empty line, assuming it's a header.
     *
     * @param filename Path to the CSV file.
     * @param skip_header If true, skip the first non-empty line (treating it as a header).
     * @param delimiter The character used to separate cells (default: ',').
     * @return [vv_string] std::vector<std::vector<std::string>> A vector of rows, each row a vector of strings.
     * @throws std::runtime_error if the file cannot be opened.
     */
    static vv_string load_csv(
        const std::string &filename,
        bool skip_header = false,
        char delimiter = ',')
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open CSV file: " + filename);
        }
        std::string line;
        bool header_skipped = false;
        vv_string data;
        while (std::getline(file, line))
        {
            trim(line);
            // Skip empty lines
            if (line.empty())
            {
                continue;
            }
            // If skipping header, skip the first non-empty line
            if (skip_header && !header_skipped)
            {
                header_skipped = true;
                continue;
            }
            // Split the line by the delimiter
            v_string row = split_line(line, delimiter);
            // If the row is empty after splitting, skip it
            if (row.empty())
            {
                continue;
            }
            data.push_back(std::move(row));
        }
        file.close();
        if (data.empty())
        {
            throw std::runtime_error("No data loaded from CSV. Check file format or path: " + filename);
        }
        return data;
    }
private:
    static void trim(std::string &s)
    {
        // Remove leading spaces
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
        // Remove trailing spaces
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                s.end());
    }
    static v_string split_line(const std::string &line, char delimiter)
    {
        v_string cells;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, delimiter))
        {
            trim(cell);
            cells.push_back(cell);
        }
        return cells;
    }
};
#endif // CSV_L_HPP