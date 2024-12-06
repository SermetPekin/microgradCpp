
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

/*

void notImplemented() {
    throw NotImplementedException();
}
 
 */

#endif //EXC_HPP
