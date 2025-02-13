#include <algorithm>
#include <chrono>
#include <cmath>
#include <complex>
#include <concepts>
#include <print>
#include <random>

// Initialize random number generator
std::random_device rd;
std::mt19937 gen(rd());

template <typename F, typename Cond, std::integral Iterations, typename Float>
    requires(std::same_as<std::invoke_result_t<F, Float>, Float> &&
        std::same_as<std::invoke_result_t<Cond, Float, Float>, bool>)
std::pair<Float, Iterations> fixed_point(
    F f, Cond cond, Iterations n, Float init)
{
    Float x = init;
    for (Iterations i = 1; i != n; ++i)
    {
        Float x1 = f(x);
        if (cond(x1, x))
            return {x1, i};
        x = x1;
    }
    return {x, n};
}

// Kernel to compute the Mandelbrot set
size_t mandelbrot(std::complex<double> c)
{
    size_t const max_iterations = 100;
    using float_type = std::complex<double>;

    auto [_, iterations] = fixed_point(
        // function to find fixed point for
        [&](float_type z) { return z = z * z + c; },
        // termination condition
        [](float_type, float_type z) { return std::abs(z) > 2.0; },
        max_iterations, float_type(0, 0));

    return iterations == max_iterations ? -1 : iterations;
}

std::complex<double> get_random_number()
{
    std::uniform_real_distribution<double> dis_x(-2, 1);
    std::uniform_real_distribution<double> dis_y(0, 2);
    return {dis_x(gen), dis_y(gen)};
}

int main()
{
    std::println("attempts,pi,error,realerror,elapsed");

    std::chrono::high_resolution_clock t;
    double real_area = 1.506591856;
    for (int max_attempts = 1; max_attempts < 30; ++max_attempts)
    {
        auto start = t.now();
        int const n_total = 1 << max_attempts;

        size_t inside_mandelbrot_set = 0;
        for (int i = 0; i != n_total; ++i)
        {
            if (mandelbrot(get_random_number()) == -1)
                ++inside_mandelbrot_set;
        }

        double area = (double(inside_mandelbrot_set) / n_total) * 12;

        std::println("{},{},{},{:%S}", n_total, area,
            fabs(area - real_area) / real_area, t.now() - start);
    }

    return 0;
}
