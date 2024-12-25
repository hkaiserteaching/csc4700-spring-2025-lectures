
#include <chrono>
#include <cmath>
#include <print>
#include <random>

#include <hpx/algorithm.hpp>
#include <hpx/hpx_main.hpp>
#include <hpx/iterator_support/counting_iterator.hpp>

#include "safe_object.hpp"

// Initialize random number generator
std::random_device rd;
thread_local std::mt19937 gen(rd());

double get_random_number()
{
    std::uniform_real_distribution<double> dis(0, 1);
    return dis(gen);
}

double sqr(double val)
{
    return val * val;
}

int main()
{
    std::println("attempts,pi,error,realerror,elapsed");

    std::chrono::high_resolution_clock t;
    double real_pi = 2 * std::acos(0.0);
    for (int max_attempts = 1; max_attempts < 30; ++max_attempts)
    {
        auto start = t.now();
        int const n_total = 1 << max_attempts;

        safe_object<std::pair<double, double>> ts({0.0, 0.0});

        hpx::for_each(hpx::execution::par, hpx::util::counting_iterator(0),
            hpx::util::counting_iterator(n_total), [&](auto) {
                double x_i = get_random_number();   // get random number in [0, 1]
                double y = 4.0 / (1.0 + sqr(x_i));
                ts.get().first += y;
                ts.get().second += sqr(y);
            });

        auto [S_n, error_term] = ts.reduce([](auto acc, auto val) {
            return std::make_pair(
                acc.first + val.first, acc.second + val.second);
        });

        double pi = S_n / n_total;
        double error =
            std::sqrt(std::fabs(error_term - n_total * sqr(pi))) / n_total;

        std::println("{},{},{},{},{:%S}", n_total, pi,
            fabs(pi - real_pi) / real_pi, error / real_pi, t.now() - start);
    }

    return 0;
}
