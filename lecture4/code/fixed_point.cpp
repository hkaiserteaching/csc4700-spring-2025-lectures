
#include <cmath>
#include <print>

// template <typename F, typename Float>
// Float fixed_point(F f, int n, Float init)
// {
//     Float x = init;
//     for (int i = 1; i != n; ++i)
//     {
//         x = f(x);
//     }
//     return x;
// }

double fixed_point(double f(double), int n, double init)
{
    double x = init;
    for (int i = 1; i != n; ++i)
    {
        x = f(x);
    }
    return x;
}

int main()
{
    for (int n = 1; n != 100; ++n)
    {
        std::println("Iterations: {}, fixed point: {}",
                     n, fixed_point([](double x) { return std::cos(x); }, n, 0.75));
    }
    return 0;
}