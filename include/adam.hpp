
#ifndef ADAM_HPP
#define ADAM_HPP
#include "value.hpp"
#include <vector>
#include <memory>
#include <cmath>
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
 /*
        Adam maintains two moving averages for each parameter:
        First Moment Estimate (Mean):
        mt=Î²1mtâˆ’1+(1âˆ’Î²1)gtmtâ€‹=Î²1â€‹mtâˆ’1â€‹+(1âˆ’Î²1â€‹)gtâ€‹
        Second Moment Estimate (Uncentered Variance):
        vt=Î²2vtâˆ’1+(1âˆ’Î²2)gt2vtâ€‹=Î²2â€‹vtâˆ’1â€‹+(1âˆ’Î²2â€‹)gt2â€‹
    It then corrects these biases and updates the parameters with:
    Î¸t=Î¸tâˆ’1âˆ’Î·â‹…m^tv^t+Ïµ
    Î¸tâ€‹=Î¸tâˆ’1â€‹âˆ’v^tâ€‹
    â€‹+ÏµÎ·â‹…m^tâ€‹â€‹
    Where:
        gtgtâ€‹: Gradient at time tt
        Î·Î·: Learning rate
        Î²1,Î²2Î²1â€‹,Î²2â€‹: Exponential decay rates
        ÏµÏµ: Smoothing term (prevents division by zero)
    */
class AdamOptimizer
{
public:
    double lr;      // Learning rate
    double beta1;   // Exponential decay rate for the first moment
    double beta2;   // Exponential decay rate for the second moment
    double epsilon; // Small constant for numerical stability
    int t;          // Time step (iteration count)
    // For storing moments for each parameter
    std::vector<std::shared_ptr<Value>> params;
    std::unordered_map<Value *, double> m; // First moment estimates
    std::unordered_map<Value *, double> v; // Second moment estimates
    // Constructor
    AdamOptimizer(std::vector<std::shared_ptr<Value>> parameters,
                  double lr = 0.001,
                  double beta1 = 0.9,
                  double beta2 = 0.999,
                  double epsilon = 1e-8)
        : lr(lr), beta1(beta1), beta2(beta2), epsilon(epsilon), t(0), params(parameters)
    {
        for (auto &param : params)
        {
            m[param.get()] = 0.0;
            v[param.get()] = 0.0;
        }
    }
    void step()
    {
        t++; //   time step
        for (auto &param : params)
        {
            double g = param->grad; // Gradient of the parameter
            //   first moment estimate (mean)
            m[param.get()] = beta1 * m[param.get()] + (1.0 - beta1) * g;
            //   second moment estimate (uncentered variance)
            v[param.get()] = beta2 * v[param.get()] + (1.0 - beta2) * g * g;
            // Compute bias-corrected estimates
            double m_hat = m[param.get()] / (1.0 - std::pow(beta1, t));
            double v_hat = v[param.get()] / (1.0 - std::pow(beta2, t));
            param->data -= lr * m_hat / (std::sqrt(v_hat) + epsilon);
        }
    }
    void zero_grad()
    {
        for (auto &param : params)
        {
            param->grad = 0.0;
        }
    }
};
#endif // ADAM_HPP