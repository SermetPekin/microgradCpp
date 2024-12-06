namespace microgradCpp
{

    using v_string = std::vector<std::string>;
    using vv_string = std::vector<std::vector<std::string>>;

    using v_double = std::vector<double>;
    using vv_double = std::vector<std::vector<double>>;

    using ColumnData = std::variant<std::vector<double>, std::vector<std::string>>;

    using vv_shared_Value = std::vector<std::vector<std::shared_ptr<Value>>>;

using DatasetType = std::vector<std::pair<std::vector<std::shared_ptr<Value>>, std::vector<std::shared_ptr<Value>>>>;
using ColRows = std::vector<std::vector<std::shared_ptr<Value>>>;

    // microgradCpp namespace
}
