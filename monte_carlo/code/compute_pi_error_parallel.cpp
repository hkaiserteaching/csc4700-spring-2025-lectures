
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
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(gen);
}

double sqr(double x)
{
    return x * x;
}

int main()
{
    std::println("attempts,pi,error,elapsed");

    std::chrono::high_resolution_clock t;
    double real_pi = 2 * std::acos(0.0);
    for (int max_attempts = 1; max_attempts < 30; ++max_attempts)
    {
        auto start = t.now();
        int const n_total = 1 << max_attempts;

        safe_object<int> nc_ts(0);

        hpx::for_each(hpx::execution::par, hpx::util::counting_iterator(0),
            hpx::util::counting_iterator(n_total), [&](auto) {
                double x = get_random_number();
                double y = get_random_number();
                if (sqr(x) + sqr(y) <= 1.0)
                {
                    ++nc_ts.get();
                }
            });

        int nc = nc_ts.reduce([](int acc, int val) { return acc + val; });

        double pi = (4.0 * nc) / n_total;
        std::println("{},{},{},{:%S}", n_total, pi,
            fabs(pi - real_pi) / real_pi, t.now() - start);
    }

    return 0;
}