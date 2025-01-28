
#include <chrono>
#include <cmath>
#include <print>
#include <random>

#include <hpx/algorithm.hpp>
#include <hpx/hpx_main.hpp>

#include "timer.hpp"

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
        timer t;
        t.start();

        int const n_total = 1 << max_attempts;
        int nc = 0;

        hpx::experimental::for_loop(hpx::execution::seq, 0, n_total,
            [&](auto) {
                double x = get_random_number();
                double y = get_random_number();
                if (sqr(x) + sqr(y) <= 1.0)
                {
                    ++nc;
                }
            });

        t.stop();

        double pi = (4.0 * nc) / n_total;
        std::println("{},{},{},{}", n_total, pi,
            fabs(pi - real_pi) / real_pi, t.elapsed());
    }

    return 0;
}