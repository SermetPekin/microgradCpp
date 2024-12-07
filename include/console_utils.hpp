#ifndef epic_failure_exit_HPP
#define epic_failure_exit_HPP

#include <cstdlib>
#include <iostream>
namespace microgradCpp
{

    void epic_failure_exit(const std::string &reason)
    {
        std::cout << "\n💥💥💥 BOOM! 💥💥💥" << std::endl;
        std::cout << "❌ Uh-oh! Something went wrong: [ 🔥 " << reason << " 🔥 ] " << std::endl;
        std::cout << "🚀 Exiting the program... like a failed rocket launch!" << std::endl;
        std::cout << "📉 Better luck next time, brave coder!" << std::endl;
        std::cout << "🔥🔥🔥 Program terminated. 🔥🔥🔥\n"
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

}

#endif // epic_failure_exit