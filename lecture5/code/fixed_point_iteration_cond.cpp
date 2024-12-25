
#include <cmath>
#include <concepts>
#include <print>

#include <boost/multiprecision/cpp_bin_float.hpp>

using big_float = boost::multiprecision::cpp_bin_float_100;

template <typename F, typename Float, typename Cond>
    requires(std::same_as<std::invoke_result_t<F, Float>, Float> &&
        std::same_as<std::invoke_result_t<Cond, Float, Float>, bool>)
Float fixed_point(F f, int n, Float init, Cond cond)
{
    Float x = init;
    for (int i = 1; i != n; ++i)
    {
        Float x1 = f(x);
        if (cond(x, x1))
            break;

        x = x1;
    }
    return x;
}

int main()
{
    using float_type = double;
    for (int n = 1; n != 17; ++n)
    {
        std::cout << n << ": fixed point: "
                  << std::setprecision(
                         std::numeric_limits<float_type>::max_digits10)
                  << fixed_point([](float_type x) { return cbrt(sin(x)); }, n,
                         float_type(1.0),
                         [](float_type curr, float_type prev) {
                             return fabs(prev - curr) < float_type(1e-10);
                         })
                  << "\n";
    }
    return 0;
}