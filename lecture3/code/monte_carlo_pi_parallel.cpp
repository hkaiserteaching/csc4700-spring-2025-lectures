
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

    double real_pi = 2 * std::acos(0.0);
    for (int max_attempts = 1; max_attempts < 30; ++max_attempts)
    {
        timer t;
        t.start();
    
        int const n_total = 1 << max_attempts;

        double S_n = 0.0, error_term = 0.0;
        hpx::experimental::for_loop(hpx::execution::par, 0, n_total,
            hpx::experimental::reduction_plus(S_n),
            hpx::experimental::reduction_plus(error_term),
            [&](auto, double& local_S_n, double& local_error_term) {
                double x_i = get_random_number();   // get random number in [0, 1]
                double y = 4.0 / (1.0 + sqr(x_i));
                local_S_n += y;
                local_error_term += sqr(y);
            });

        t.stop();

        double pi = S_n / n_total;
        double error =
            std::sqrt(std::fabs(error_term - n_total * sqr(pi))) / n_total;

        std::println("{},{},{},{},{}", n_total, pi,
            fabs(pi - real_pi) / real_pi, error / real_pi, t.elapsed());
    }

    return 0;
}
