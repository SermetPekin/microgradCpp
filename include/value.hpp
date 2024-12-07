#ifndef VALUE_HPP
#define VALUE_HPP

/*
MIT License

Copyright (c) [2024] Sermet Pekin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <cmath>

#include "exceptions.hpp"

class Value : public std::enable_shared_from_this<Value>
{
public:
    double data;       // Scalar value
    double grad;       // Gradient value
    std::string label; // Optional label for debugging
    std::vector<std::shared_ptr<Value>> parents;
    std::function<void()> _backward; // Backward function for autograd

    // Constructor
    constexpr Value(double data, const std::string &label = "")
        : data(data), grad(0.0), label(label), _backward([]() {}) {}

    // Copy constructor
    constexpr Value(const Value &other)
        : data(other.data), grad(other.grad), label(other.label), parents(other.parents), _backward(other._backward) {}

    // Add a parent to the computational graph (ensuring no duplicates)
    void add_parent(const std::shared_ptr<Value> &parent)
    {
        if (std::find(parents.begin(), parents.end(), parent) == parents.end())
        {
            parents.push_back(parent);
        }
    }

    // Build topological order for backpropagation
    std::vector<std::shared_ptr<Value>> build_topological_order()
    {
        std::vector<std::shared_ptr<Value>> topo;
        std::unordered_set<Value *> visited;

        std::function<void(Value *)> visit = [&](Value *v)
        {
            if (v && visited.find(v) == visited.end())
            {
                visited.insert(v);
                for (auto &p : v->parents)
                {
                    visit(p.get());
                }
                topo.push_back(v->shared_from_this());
            }
        };

        visit(this);
        return topo;
    }

    // Backward propagation
    void backward(double grad_init = 1.0)
    {
        grad = grad_init; // Seed gradient for the output node
        auto topo = build_topological_order();
        for (auto it = topo.rbegin(); it != topo.rend(); ++it)
        {
            (*it)->_backward();
        }
    }

    // Unary operations

    // Logarithm
    std::shared_ptr<Value> log()
    {
        auto out = std::make_shared<Value>(std::log(data));
        out->add_parent(shared_from_this());
        out->_backward = [self = shared_from_this(), out]()
        {
            self->grad += (1.0 / self->data) * out->grad;
        };
        return out;
    }

    // Exponent
    std::shared_ptr<Value> exp()
    {

        // throw NotImplementedException();

        auto out = std::make_shared<Value>(std::exp(data));
        out->add_parent(shared_from_this());
        out->_backward = [self = shared_from_this(), out]()
        {
            self->grad += out->data * out->grad; // d/dx exp(x) = exp(x)
        };
        return out;
    }

    // Power with a double exponent
    std::shared_ptr<Value> pow(double exponent)
    {
        auto out = std::make_shared<Value>(std::pow(data, exponent));
        out->add_parent(shared_from_this());
        out->_backward = [self = shared_from_this(), out, exponent]()
        {
            if (self->data != 0.0)
            {
                self->grad += exponent * std::pow(self->data, exponent - 1) * out->grad;
            }
        };
        return out;
    }

    // Debug print
    friend std::ostream &operator<<(std::ostream &os, const Value &v)
    {
        os << "Value(data=" << v.data << ", grad=" << v.grad;
        if (!v.label.empty())
            os << ", label='" << v.label << "'";
        os << ", parents=" << v.parents.size() << ")";
        return os;
    }

    // Value Class constructor
};

// Operator Overloads (assuming both operands are std::shared_ptr<Value>)
// ========================================================================
//    Addition Templated Functions
// ========================================================================
// +[V, V ]
inline std::shared_ptr<Value> operator+(const std::shared_ptr<Value> &lhs, const std::shared_ptr<Value> &rhs)
{
    auto out = std::make_shared<Value>(lhs->data + rhs->data);
    out->add_parent(lhs);
    out->add_parent(rhs);
    out->_backward = [lhs, rhs, out]()
    {
        lhs->grad += out->grad;
        rhs->grad += out->grad;
    };
    return out;
}

// +[V , T]
template <typename T>
std::shared_ptr<Value> operator+(const std::shared_ptr<Value> &lhs, const T &rhs)
{
    double rhs_data = rhs; // rhs is a double in this case
    if constexpr (std::is_same_v<T, std::shared_ptr<Value>>)
    {
        rhs_data = rhs->data; // If rhs is a std::shared_ptr<Value>, extract the data
    }

    auto out = std::make_shared<Value>(lhs->data + rhs_data);
    out->add_parent(lhs);
    if constexpr (std::is_same_v<T, std::shared_ptr<Value>>)
    {
        out->add_parent(rhs); // If rhs is a std::shared_ptr<Value>, add it as a parent
    }
    out->_backward = [lhs, rhs, out]()
    {
        lhs->grad += out->grad;
        if constexpr (std::is_same_v<T, std::shared_ptr<Value>>)
        {
            rhs->grad += out->grad;
        }
    };
    return out;
}

// +[T , V]
template <typename T>
std::shared_ptr<Value> operator+(const T &lhs, const std::shared_ptr<Value> &rhs)
{
    return rhs + lhs; // Reuse the above operator to avoid duplication
}



// ========================================================================
//    Subtraction for two std::shared_ptr<Value> operands
// ========================================================================
// -[V , V]
inline std::shared_ptr<Value> operator-(const std::shared_ptr<Value> &lhs, const std::shared_ptr<Value> &rhs)
{
    auto out = std::make_shared<Value>(lhs->data - rhs->data);
    out->add_parent(lhs);
    out->add_parent(rhs);
    out->_backward = [lhs, rhs, out]()
    {
        lhs->grad += out->grad;
        rhs->grad -= out->grad;
    };
    return out;
}

// -[V , T]
template <typename T>
std::shared_ptr<Value> operator-(const std::shared_ptr<Value> &lhs, const T &rhs)
{
    double rhs_data = rhs; // rhs is a double in this case
    if constexpr (std::is_same_v<T, std::shared_ptr<Value>>)
    {
        rhs_data = rhs->data; // If rhs is a std::shared_ptr<Value>, extract the data
    }

    auto out = std::make_shared<Value>(lhs->data - rhs_data);
    out->add_parent(lhs);
    if constexpr (std::is_same_v<T, std::shared_ptr<Value>>)
    {
        out->add_parent(rhs); // If rhs is a std::shared_ptr<Value>, add it as a parent
    }
    out->_backward = [lhs, rhs, out]()
    {
        lhs->grad += out->grad;
        if constexpr (std::is_same_v<T, std::shared_ptr<Value>>)
        {
            rhs->grad -= out->grad;
        }
    };
    return out;
}

// -[T , V]
template <typename T>
std::shared_ptr<Value> operator-(const T &lhs, const std::shared_ptr<Value> &rhs)
{
    return rhs - lhs; // Reuse the above operator to avoid duplication
}

// ========================================================================
//    Multiplication
// ========================================================================
// *[V , V]
inline std::shared_ptr<Value> operator*(const std::shared_ptr<Value> &lhs, const std::shared_ptr<Value> &rhs)
{
    auto out = std::make_shared<Value>(lhs->data * rhs->data);
    out->add_parent(lhs);
    out->add_parent(rhs);
    out->_backward = [lhs, rhs, out]()
    {
        lhs->grad += rhs->data * out->grad;
        rhs->grad += lhs->data * out->grad;
    };
    return out;
}

// *[V , T]
template <typename T>
std::shared_ptr<Value> operator*(const std::shared_ptr<Value> &lhs, const T &rhs)
{
    double rhs_data = rhs; // rhs is a double in this case
    if constexpr (std::is_same_v<T, std::shared_ptr<Value>>)
    {
        rhs_data = rhs->data; // If rhs is a std::shared_ptr<Value>, extract the data
    }

    auto out = std::make_shared<Value>(lhs->data * rhs_data);
    out->add_parent(lhs);
    if constexpr (std::is_same_v<T, std::shared_ptr<Value>>)
    {
        out->add_parent(rhs); // If rhs is a std::shared_ptr<Value>, add it as a parent
    }

    out->_backward = [lhs, rhs, out]() {    // Capture rhs_data explicitly
        lhs->grad += rhs->data * out->grad; // Use rhs_data here safely
        if constexpr (std::is_same_v<T, std::shared_ptr<Value>>)
        {
            rhs->grad += lhs->data * out->grad;
        }
    };

    return out;
}

// *[T , V]
template <typename T>
std::shared_ptr<Value> operator*(const T &lhs, const std::shared_ptr<Value> &rhs)
{
    return rhs * lhs; // Reuse the previous operator to avoid duplication
}
// Multiplication
// std::shared_ptr<Value> operator*(const std::shared_ptr<Value>& lhs, const std::shared_ptr<Value>& rhs) {
//     auto out = std::make_shared<Value>(lhs->data * rhs->data);
//     out->add_parent(lhs);
//     out->add_parent(rhs);
//     out->_backward = [lhs, rhs, out]() {
//         lhs->grad += rhs->data * out->grad;
//         rhs->grad += lhs->data * out->grad;
//     };
//     return out;
// }

// ========================================================================
//    Division Templated Functions
// ========================================================================
// /[V , V]
inline std::shared_ptr<Value> operator/(const std::shared_ptr<Value> &lhs, const std::shared_ptr<Value> &rhs)
{
    auto out = std::make_shared<Value>(lhs->data / rhs->data);
    out->add_parent(lhs);
    out->add_parent(rhs);
    out->_backward = [lhs, rhs, out]()
    {
        lhs->grad += (1.0 / rhs->data) * out->grad;
        rhs->grad -= (lhs->data / (rhs->data * rhs->data)) * out->grad;
    };
    return out;
}

// /[V , T]
template <typename T>
std::shared_ptr<Value> operator/(const std::shared_ptr<Value> &lhs, const T &rhs)
{
    std::shared_ptr<Value> rhs_value;
    if constexpr (std::is_same_v<T, double>)
    {
        rhs_value = std::make_shared<Value>(rhs);
    }
    else
    {
        rhs_value = rhs;
    }

    return lhs / rhs_value;
}

// /[T , V]
template <typename T>
std::shared_ptr<Value> operator/(const T &lhs, const std::shared_ptr<Value> &rhs)
{
    std::shared_ptr<Value> lhs_value = std::make_shared<Value>(lhs);
    return rhs / lhs_value; // Reuse the previous operator
}

 

// Division
// std::shared_ptr<Value> operator/(const std::shared_ptr<Value>& lhs, const std::shared_ptr<Value>& rhs) {
//     auto out = std::make_shared<Value>(lhs->data / rhs->data);
//     out->add_parent(lhs);
//     out->add_parent(rhs);
//     out->_backward = [lhs, rhs, out]() {
//         lhs->grad += (1.0 / rhs->data) * out->grad;
//         rhs->grad -= (lhs->data / (rhs->data * rhs->data)) * out->grad;
//     };
//     return out;
// }

// Unary Negation
inline std::shared_ptr<Value> operator-(const std::shared_ptr<Value> &lhs)
{
    auto out = std::make_shared<Value>(-lhs->data);
    out->add_parent(lhs);
    out->_backward = [lhs, out]()
    {
        lhs->grad -= out->grad;
    };
    return out;
}

#endif // VALUE_HPP
