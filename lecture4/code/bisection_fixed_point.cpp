
#include <cmath>
#include <iomanip>
#include <iostream>

// Defining equation to be solved.
// Change this equation to solve another problem.
double f(double x)
{
    return 3 * x - std::cos(x) - 1;
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
        Float x1 = f(x);    // get next iteration result
        if (cond(x1, x))    // compare with previous iteration result
            return {x1, i};
        x = x1;             // update for next iteration
    }
    return {x, n};
}

auto bisection(double f(double), size_t max_iterations, double xn_init,
    double xm_init, double epsilon)
{
    using value_type = std::pair<double, double>;
    return fixed_point(
        [&](value_type bounds) {
            double fn = f(bounds.first), fm = f(bounds.second);
            double next_x = (bounds.first + bounds.second) / 2;
            if (std::signbit(fn) == std::signbit(f(next_x)))
                bounds.first = next_x;
            else
                bounds.second = next_x;
            return bounds;
        },
        [&](value_type, value_type curr) {
            return std::fabs(f(curr.first)) <= epsilon ||
                std::fabs(f(curr.second)) <= epsilon;
        },
        max_iterations, std::make_pair(xn_init, xm_init));
}

int main()
{
    // Inputs
    double xn = 0, xm = 1;
    std::cout << "Enter initial lower guess: ";
    std::cin >> xn;

    std::cout << "Enter initial upper guess: ";
    std::cin >> xm;

    std::cout << xn << ", " << xm << "\n";

    if (std::signbit(f(xn)) == std::signbit(f(xm)))
    {
        std::cout << "Lower and upper guesses should produce function values "
                     "of different sign.";
        return -1;
    }

    double epsilon = 1e-6;
    std::cout << "Enter tolerable error: ";
    std::cin >> epsilon;

    unsigned int max_iterations = 100;
    std::cout << "Enter maximum number of iterations: ";
    std::cin >> max_iterations;

    std::cout << "*********************\n"
              << "Bisection Method\n"
              << "*********************\n";

    auto [result, iterations] = bisection(f, max_iterations, xn, xm, epsilon);
    if (iterations == max_iterations)
    {
        std::cout << "Method diverges, couldn't find root\n";
    }
    else
    {
        // found approximation of root
        std::cout << "\nRoot is: " << result.first << "\n";
    }

    return 0;
}