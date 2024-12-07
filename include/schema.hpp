#ifndef SCHEMA_HPP
#define SCHEMA_HPP


#include <vector>
#include <string>
#include <unordered_map>

class Schema {
public:
    std::vector<std::string> columns;
    std::vector<std::string> types;  // e.g., "double", "string", "category"

    Schema(const std::vector<std::string>& columns, const std::vector<std::string>& types)
        : columns(columns), types(types) {}

    size_t get_column_index(const std::string& name) const {
        for (size_t i = 0; i < columns.size(); ++i) {
            if (columns[i] == name) return i;
        }
        throw std::runtime_error("Column not found: " + name);
    }
};


#endif // SCHEMA_HPP