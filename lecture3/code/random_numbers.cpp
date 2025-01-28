#include <cmath>
#include <print>
#include <random>

#include "histogram.hpp"
#include "matplotlibcpp.h"

// Initialize random number generator
std::random_device rd;
std::mt19937 gen(rd());

int get_random_number()
{
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(gen);
}

namespace plt = matplotlibcpp;

int main()
{
    plt::figure_size(1280, 960); // Initialize a 1280 X 960 figure

    for (int max_attempts = 1; max_attempts < 30; ++max_attempts)
    {
        int const n_total = 1 << max_attempts;

        histogram<double> hist(100, 0.0, 1.0);
        for (int n = 0; n < n_total; ++n)
        {
            hist.add_value(get_random_number());
        }
        plt::plot(hist.buckets(), hist.data());

        std::println("{}", n_total);
    }
    plt::save("random_numbers.png");
}