
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

// Stable softmax with +inf early handling:
// - If any +inf present: split mass equally among all +inf entries (others get 0).
// - Else do the standard "max trick" for stability.
// - If everything is -inf (or non-finite but no +inf), return uniform as a practical fallback.
std::vector<double> softmax(const std::vector<double>& x) {
    const std::size_t n = x.size();
    if (n == 0) return {};

    // Early: detect any +infinity
    bool any_pos_inf = false;
    for (double v : x) {
        if (v == std::numeric_limits<double>::infinity()) {
            any_pos_inf = true;
            break;
        }
    }
    if (any_pos_inf) {
        // Split probability mass equally among all +inf entries.
        std::size_t k = 0;
        for (double v : x) if (v == std::numeric_limits<double>::infinity()) ++k;
        std::vector<double> p(n, 0.0);
        const double share = 1.0 / static_cast<double>(k);
        for (std::size_t i = 0; i < n; ++i)
            if (x[i] == std::numeric_limits<double>::infinity()) p[i] = share;
        return p;
    }

    // Normal finite case (or no +inf). Find max for stability.
    double m = *std::max_element(x.begin(), x.end());

    // If m is not finite (e.g., all -inf or there are NaNs but no +inf), return uniform.
    if (!std::isfinite(m)) {
        return std::vector<double>(n, 1.0 / static_cast<double>(n));
    }

    // Exponentiate shifted values and sum with Neumaier compensation.
    std::vector<double> p(n);
    double sum = 0.0, c = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        double yi = std::exp(x[i] - m);
        p[i] = yi;
        double t = sum + yi;
        if (std::fabs(sum) >= std::fabs(yi)) c += (sum - t) + yi; else c += (yi - t) + sum;
        sum = t;
    }
    sum += c;

    // Defensive fallback (shouldn't trigger with the max trick, but just in case)
    if (!(sum > 0.0) || !std::isfinite(sum)) {
        return std::vector<double>(n, 1.0 / static_cast<double>(n));
    }

    // Normalize
    for (double& v : p) v /= sum;
    return p;
}


#include <iostream>
int main() {
    std::vector<double> a{1.0, 2.0, 3.0};
    auto p = softmax(a);
    for (double v : p) std::cout << v << " ";
    // ~ 0.0900306 0.244728 0.665241
    std::cout << "\n";

    std::vector<double> b{1000.0, 1001.0};
    p = softmax(b);
    for (double v : p) std::cout << v << " ";
    // ~ 0.268941 0.731059
    std::cout << "\n";
}
