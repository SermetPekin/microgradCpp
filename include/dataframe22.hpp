#ifndef DATAFRAME_HPP
#define DATAFRAME_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <stdexcept>

class DataFrame {
private:
    std::vector<std::string> column_names;
    std::vector<std::vector<std::string>> data;

public:
    // Constructor
    DataFrame(const std::vector<std::string>& columns)
        : column_names(columns) {}

    // Add a row to the DataFrame
    void add_row(const std::vector<std::string>& row) {
        if (row.size() != column_names.size()) {
            throw std::invalid_argument("Row size does not match number of columns");
        }
        data.push_back(row);
    }

    // Display the DataFrame
    void display(size_t num_rows = 5) const {
        size_t rows_to_display = std::min(num_rows, data.size());

        // Print column names
        for (const auto& col : column_names) {
            std::cout << std::setw(12) << col << " ";
        }
        std::cout << "\n" << std::string(12 * column_names.size(), '-') << "\n";

        // Print rows
        for (size_t i = 0; i < rows_to_display; ++i) {
            for (const auto& cell : data[i]) {
                std::cout << std::setw(12) << cell << " ";
            }
            std::cout << "\n";
        }

        if (rows_to_display < data.size()) {
            std::cout << "...\n";
        }
    }

    // Get number of rows
    size_t num_rows() const {
        return data.size();
    }

    // Get number of columns
    size_t num_columns() const {
        return column_names.size();
    }

    // Save DataFrame to CSV
    void save_to_csv(const std::string& file_path) const {
        std::ofstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for writing");
        }

        // Write header
        for (size_t i = 0; i < column_names.size(); ++i) {
            file << column_names[i];
            if (i != column_names.size() - 1) {
                file << ",";
            }
        }
        file << "\n";

        // Write rows
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i != row.size() - 1) {
                    file << ",";
                }
            }
            file << "\n";
        }

        file.close();
    }

    // Load DataFrame from CSV
    static DataFrame load_from_csv(const std::string& file_path, bool has_header = true) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for reading");
        }

        std::string line;
        std::vector<std::string> columns;
        std::vector<std::vector<std::string>> rows;

        // Read the header
        if (has_header && std::getline(file, line)) {
            std::istringstream ss(line);
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                columns.push_back(cell);
            }
        }

        // Read the data
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string cell;
            std::vector<std::string> row;
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            rows.push_back(row);
        }

        file.close();

        // Create DataFrame
        DataFrame df(columns.empty() ? std::vector<std::string>(rows[0].size()) : columns);
        for (const auto& row : rows) {
            df.add_row(row);
        }

        return df;
    }
};

#endif // DATAFRAME_HPP

