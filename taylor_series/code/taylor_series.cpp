
#include <cstddef>
#include <cmath>

double exp_taylor_series(double x, size_t N)
{
    size_t fact = 1;
    double value = 1.0;
    for (size_t n = 1; n != N; n++)
    {
        value += std::pow(x, n) / fact;
        fact *= n;
    }
    return value;
}

double sin_taylor_series(double x, size_t N)
{
    size_t fact = 1;
    double value = 1.0;
    for (size_t n = 1; n != N; n++)
    {
        value += std::pow(x, n) / fact;
        fact *= n;
    }
    return value;
}

int main()
{
    return 0;
}