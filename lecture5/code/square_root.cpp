
#include <cmath>
#include <print>

template <typename Float>
Float average(Float x1, Float x2)
{
    return (x1 + x2) / 2;
}

template <typename Float>
Float iterate_sqrt(Float a, int n, Float init)
{
    Float x = init;
    for (int i = 1; i != n; ++i)
    {
        x = average(x, a / x);
    }
    return x;
}

int main()
{
    for (int n = 1; n != 10; ++n)
    {
        std::println("Iterations: {}, approximation error: {}", 
            n, iterate_sqrt(2.0, n, 1.0) - std::sqrt(2.0));
    }
    return 0;
}