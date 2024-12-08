#ifndef ADAM_HPP
#define ADAM_HPP

#include "value.hpp"
#include <vector>
#include <memory>
#include <cmath>

class AdamOptimizer {
public:
    double lr;         // Learning rate
    double beta1;      // Exponential decay rate for the first moment
    double beta2;      // Exponential decay rate for the second moment
    double epsilon;    // Small constant for numerical stability
    int t;             // Time step (iteration count)

    // For storing moments for each parameter
    std::vector<std::shared_ptr<Value>> params;
    std::unordered_map<Value*, double> m; // First moment estimates
    std::unordered_map<Value*, double> v; // Second moment estimates

    // Constructor
    AdamOptimizer(std::vector<std::shared_ptr<Value>> parameters,
                  double lr = 0.001,
                  double beta1 = 0.9,
                  double beta2 = 0.999,
                  double epsilon = 1e-8)
        : lr(lr), beta1(beta1), beta2(beta2), epsilon(epsilon), t(0), params(parameters) {
        for (auto& param : params) {
            m[param.get()] = 0.0;
            v[param.get()] = 0.0;
        }
    }

    // Step function to update parameters
    void step() {
        t++; // Increment time step
        for (auto& param : params) {
            double g = param->grad; // Gradient of the parameter

            // Update first moment estimate (mean)
            m[param.get()] = beta1 * m[param.get()] + (1.0 - beta1) * g;

            // Update second moment estimate (uncentered variance)
            v[param.get()] = beta2 * v[param.get()] + (1.0 - beta2) * g * g;

            // Compute bias-corrected estimates
            double m_hat = m[param.get()] / (1.0 - std::pow(beta1, t));
            double v_hat = v[param.get()] / (1.0 - std::pow(beta2, t));

            // Update parameter
            param->data -= lr * m_hat / (std::sqrt(v_hat) + epsilon);
        }
    }

    // Zero gradients for the next step
    void zero_grad() {
        for (auto& param : params) {
            param->grad = 0.0;
        }
    }
};

#endif // ADAM_HPP