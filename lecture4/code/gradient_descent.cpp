
#include <cmath>
#include <concepts>
#include <iomanip>
#include <iostream>

// Defining equation to be solved.
// Change this equation to solve another problem.
double f(double x)
{
    return 3 * x * x - 5;
}

// Calculate slope of f(x) at x.
double f_prime(double f(double), double x, double epsilon)
{
    double delta = epsilon / 50;
    return (f(x + delta) - f(x - delta)) / (2 * delta);
}

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

std::pair<double, unsigned int> gradient_descent(double f(double), double xn,
    unsigned int max_iterations = 100, double epsilon = 0.0001,
    double learning_rate = 0.01)
{
    return fixed_point(
        [&](double xn) {
            double gn = f_prime(f, xn, epsilon);
            return xn - learning_rate * gn;
        },
        [&](double prev, double curr) { return fabs(curr - prev) <= epsilon; },
        max_iterations, xn);
}

int main()
{
    // Inputs
    double xn = 2.0;
    //std::cout << "Enter initial guess: ";
    //std::cin >> xn;

    double epsilon = 0.00001;
    //std::cout << "Enter tolerable error: ";
    //std::cin >> epsilon;

    unsigned int max_iterations = 1000;
    //std::cout << "Enter maximum number of iterations: ";
    //std::cin >> max_iterations;

    std::cout << "\n"
              << "***********************\n"
              << "Gradient Descent Method\n"
              << "***********************\n";

    try
    {
        auto [min_value, iterations] =
            gradient_descent(f, xn, max_iterations, epsilon);
        if (iterations == max_iterations)
            throw std::runtime_error("Method diverges, couldn't find root");

        std::cout << "\nMinumum is at: " << min_value << "\n";
    }
    catch (std::runtime_error const& e)
    {
        std::cout << e.what() << "\n";
    }

    return 0;
}