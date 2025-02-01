
#include <cmath>
#include <concepts>
#include <print>

#include <boost/multiprecision/cpp_bin_float.hpp>

using big_float = boost::multiprecision::cpp_bin_float_100;

template <typename F, typename Float>
    requires(std::same_as<std::invoke_result_t<F, Float>, Float>)
Float fixed_point(F f, int n, Float init, Float epsilon = 1e-10)
{
    Float x = init;
    for (int i = 1; i != n; ++i)
    {
        Float x1 = f(x);
        Float error = fabs(x1 - x);
        
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
    using float_type = double;
    for (int n = 1; n != 17; ++n) {
        std::cout << n << ": fixed point: " 
            << std::setprecision(std::numeric_limits<float_type>::max_digits10)
            << fixed_point([](float_type x) { return cbrt(sin(x)); }, 
                           n, float_type(1.0))
            << "\n";
    }
    return 0;
}