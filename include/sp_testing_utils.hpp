#ifndef SPTEST_HPP
#define SPTEST_HPP

#include <vector>
#include <algorithm>
#include <fstream>

namespace sptest
{

    static inline void create_temp_csv(const std::string &filename, const std::string &content)
    {
        std::ofstream file(filename);
        // ASSERT_TRUE(file.is_open());
        file << content;
        file.close();
    }

    template <typename T, typename U>
    static inline bool contains(const std::vector<T> &vec, const U &item)
    {

        try
        {
            T x = static_cast<T>(item);
            return std::find(vec.begin(), vec.end(), x) != vec.end();
        }
        catch (...)
        {

            return false;
        }
    }

}

#endif // SPTEST_HPP
