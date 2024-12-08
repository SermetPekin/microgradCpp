#ifndef epic_failure_exit_HPP
#define epic_failure_exit_HPP

#include <cstdlib>
#include <iostream>
namespace microgradCpp
{
    inline void epic_failure_exit(const std::string &reason, bool throw_ = true)
    {
        std::cout << "\n💥💥💥 BOOM! 💥💥💥" << std::endl;
        std::cout << "❌ Uh-oh! Something went wrong: [ 🔥 " << reason << " 🔥 ] " << std::endl;
        std::cout << "🚀 Exiting the program... like a failed rocket launch!" << std::endl;
        std::cout << "📉 Better luck next time, brave coder!" << std::endl;
        std::cout << "🔥🔥🔥 Program terminated. 🔥🔥🔥\n"
                  << std::endl;
        if (throw_)
        {
            throw std::runtime_error(reason);
        }

        std::exit(EXIT_FAILURE);
    }

    inline void epic_out_of_range(const std::string &reason )
    {
        std::cout << "\n💥💥💥 BOOM! 💥💥💥" << std::endl;
        std::cout << "❌ Uh-oh! Something went wrong: [ 🔥 " << reason << " 🔥 ] " << std::endl;
        std::cout << "🚀 Exiting the program... like a failed rocket launch!" << std::endl;
        std::cout << "📉 Better luck next time, brave coder!" << std::endl;
        std::cout << "🔥🔥🔥 Program terminated. 🔥🔥🔥\n"
                  << std::endl;
    
        throw std::out_of_range(reason) ; 
    }

    // Function to format shapes for display

    inline std::string format_shape(size_t rows, size_t cols)
    {
        std::ostringstream oss;
        oss << rows << " x " << cols;
        return oss.str();
    }

    inline void display_header(const std::string &message)
    {
        std::cout << "\n====================================================\n";
        std::cout << "🚨 " << message << "\n";
        std::cout << "====================================================\n";
    }

    inline void display_shapes(const std::string &label, size_t input_size, size_t output_size)
    {
        std::cout << "📊 " << label << " Shape: [" << input_size << " features -> " << output_size << " targets]\n";
        std::cout << "----------------------------------------------------\n";
    }

}

#endif // epic_failure_exit