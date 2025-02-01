
#include <cmath>
#include <concepts>
#include <iomanip>
#include <iostream>

// Defining equation to be solved.
// Change this equation to solve another problem.
double f(double x)
{
    return 3 * x - std::cos(x) - 1;
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

std::pair<double, unsigned int> newton(double f(double), double xn,
    unsigned int max_iterations = 100, double epsilon = 0.0001)
{
    return fixed_point(
        [&](double xn) {
            double gn = f_prime(f, xn, epsilon);
            if (gn == 0.0)
            {
                throw std::runtime_error(
                    "Derivative is equal to zero, can't use this method.");
            }
            return xn - f(xn) / gn;
        },
        [&](double, double curr) { return fabs(f(curr)) <= epsilon; },
        max_iterations, xn);
}

int main()
{
    // Inputs
    double xn;
    std::cout << "Enter initial guess: ";
    std::cin >> xn;

    double epsilon;
    std::cout << "Enter tolerable error: ";
    std::cin >> epsilon;

    unsigned int max_iterations;
    std::cout << "Enter maximum number of iterations: ";
    std::cin >> max_iterations;

    std::cout << "\n"
              << "*********************\n"
              << "Newton Raphson Method\n"
              << "*********************\n";

    try
    {
        auto [root, iterations] = newton(f, xn, max_iterations, epsilon);
        if (iterations == max_iterations)
            throw std::runtime_error("Method diverges, couldn't find root");

        std::cout << "\nRoot is: " << root << "\n";
    }
    catch (std::runtime_error const& e)
    {
        std::cout << e.what() << "\n";
    }

    return 0;
}