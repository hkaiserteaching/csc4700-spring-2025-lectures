
#include <algorithm>
#include <concepts>
#include <functional>
#include <iterator>
#include <numeric>
#include <print>
#include <vector>

template <std::integral T>
T recursive_choose(T n, T k)
{
    if (n < k)
        return T(0);    // view elements out of the triangle as 0
    if (k == T(0))
        return T(1);
    return T((n * recursive_choose(n - T(1), k - T(1))) / k);
}

template <std::integral T>
T iterative_choose(T n, T k)
{
    if (n < k)
        return T(0);
    n -= k;
    if (n < k)
        std::swap(n, k);

    T result(1);
    T i(0);
    while (i < k)
    {
        ++i;
        ++n;
        result *= n;    // prone to overflows!
        result /= i;
    }
    return result;
}

template <std::integral T>
T iterative_choose_gcd(T n, T k)
{
    if (n < k)
        return T(0);
    n -= k;
    if (n < k)
        std::swap(n, k);

    T result(1);
    T i(0);
    while (i < k)
    {
        ++i;
        ++n;

        T d(std::gcd(n, i));
        result /= (i / d);
        result *= (n / d);
    }
    return result;
}

int main()
{
    std::println("{}", recursive_choose(10, 4));
    std::println("{}", iterative_choose(10, 4));
    std::println("{}", iterative_choose_gcd(10, 4));
}