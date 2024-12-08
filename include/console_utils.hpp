#ifndef epic_failure_exit_HPP
#define epic_failure_exit_HPP
// MIT License

// Copyright (c) [2024] Sermet Pekin

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// */
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
  
    inline void epic_out_of_range(const std::string &reason)
    {
        std::cout << "\n💥💥💥 BOOM! 💥💥💥" << std::endl;
        std::cout << "❌ Uh-oh! Something went wrong: [ 🔥 " << reason << " 🔥 ] " << std::endl;
        std::cout << "🚀 Exiting the program... like a failed rocket launch!" << std::endl;
        std::cout << "📉 Better luck next time, brave coder!" << std::endl;
        std::cout << "🔥🔥🔥 Program terminated. 🔥🔥🔥\n"
                  << std::endl;

        throw std::out_of_range(reason);
    }
  inline void stop(const std::string &reason = "...")
    {
        epic_out_of_range(reason);
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