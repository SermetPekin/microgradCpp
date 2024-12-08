
#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include <stdexcept>

#include <iostream>
#include <stdexcept>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath> // For std::abs

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>

class Range
{
public:
    class Iterator
    {
    public:
        Iterator(int current, int step) : current_(current), step_(step) {}
        int operator*() const { return current_; }
        Iterator &operator++()
        {
            current_ += step_;
            return *this;
        }
        bool operator!=(const Iterator &other) const
        {
            return step_ > 0 ? current_ < other.current_ : current_ > other.current_;
        }

    private:
        int current_;
        int step_;
    };

    Range(int end) : start_(0), end_(end), step_(1) {}
    Range(int start, int end) : start_(start), end_(end), step_(1) {}
    Range(int start, int end, int step) : start_(start), end_(end), step_(step)
    {
        if (step == 0)
            throw std::invalid_argument("Step cannot be zero");
    }

    Iterator begin() const { return Iterator(start_, step_); }
    Iterator end() const { return Iterator(end_, step_); }

 

    std::vector<int> to_vector() const
    {
        return to_vector<int>();
    }
 
    template <typename T>
    std::vector<T> to_vector() const
    {
        std::vector<T> result;
        for (int value : *this)
        {
            result.push_back(static_cast<T>(value));
        }
        return result;
    }

 

    bool includes(int number) const
    {
        return step_ > 0 ? number >= start_ && number < end_ && (number - start_) % step_ == 0
                         : number <= start_ && number > end_ && (start_ - number) % std::abs(step_) == 0;
    }

private:
    int start_;
    int end_;
    int step_;
};
