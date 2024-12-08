
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
#include "micrograd.hpp"
#include <gtest/gtest.h>
#include <memory>

#include <gtest/gtest.h>
#include "range.hpp"  // Assuming the Range class is defined in range.hpp

// Test for Range(end)
TEST(RangeTest, RangeWithEndOnly) {
    Range r(5);
    std::vector<int> expected = {0, 1, 2, 3, 4};
    EXPECT_EQ(r.to_vector(), expected);
}

// Test for Range(start, end)
TEST(RangeTest, RangeWithStartAndEnd) {
    Range r(2, 7);
    std::vector<int> expected = {2, 3, 4, 5, 6};
    EXPECT_EQ(r.to_vector(), expected);
}

// Test for Range(start, end, step)
TEST(RangeTest, RangeWithStartEndAndStep) {
    Range r(1, 10, 2);
    std::vector<int> expected = {1, 3, 5, 7, 9};
    EXPECT_EQ(r.to_vector(), expected);
}

// Test for backward range with negative step
TEST(RangeTest, RangeWithNegativeStep) {
    Range r(10, 1, -2);
    std::vector<int> expected = {10, 8, 6, 4, 2};
    EXPECT_EQ(r.to_vector(), expected);
}

// Test for empty range when start == end
TEST(RangeTest, EmptyRangeWhenStartEqualsEnd) {
    Range r(5, 5);
    std::vector<int> expected = {};
    EXPECT_EQ(r.to_vector(), expected);
}

// Test for empty range when step direction doesn't reach end
TEST(RangeTest, EmptyRangeDueToStepDirection) {
    Range r(5, 10, -1);
    std::vector<int> expected = {};
    EXPECT_EQ(r.to_vector(), expected);
}

// Test for includes method with positive step
TEST(RangeTest, IncludesWithPositiveStep) {
    Range r(0, 10, 2);
    EXPECT_TRUE(r.includes(4));
    EXPECT_FALSE(r.includes(5));
    EXPECT_TRUE(r.includes(0));
    EXPECT_FALSE(r.includes(9));
}

// Test for includes method with negative step
TEST(RangeTest, IncludesWithNegativeStep) {
    Range r(10, 0, -2);
    EXPECT_TRUE(r.includes(4));
    EXPECT_FALSE(r.includes(5));
    EXPECT_TRUE(r.includes(10));
    EXPECT_FALSE(r.includes(1));
}

// Test for invalid step (zero step)
TEST(RangeTest, InvalidStep) {
    EXPECT_THROW(Range(0, 10, 0), std::invalid_argument);
}
