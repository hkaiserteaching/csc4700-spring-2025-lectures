
#include <cmath>
#include <print>

template <typename Float>
Float average(Float x1, Float x2)
{
    return (x1 + x2) / 2;
}

template <typename F, typename Float>
Float fixed_point(F f, int n, Float init, Float epsilon = 1e-10)
{
    Float x = init;
    for (int i = 1; i != n; ++i)
    {
        double x1 = f(x);
        double error = std::fabs(x1 - x);
        
        x = x1;

        if (error < epsilon)
        {
            break;
        }
    }
    return x;
}

int main()
{
    auto average = [a = 2.0](double x)
    { return ::average(x, a / x); };

    for (int n = 1; n != 10; ++n)
    {
        std::println("Iterations: {}, approximation error: {}",
                     n, fixed_point(average, n, 1.0) - std::sqrt(2.0));
    }
    return 0;
}