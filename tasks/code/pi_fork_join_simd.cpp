
#include <numeric>
#include <print>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <hpx/algorithm.hpp>
#include <hpx/datapar.hpp>
#include <hpx/hpx_main.hpp>
#include <hpx/iterator_support/counting_iterator.hpp>

//#include "counting_iterator.hpp"
#include "safe_object.hpp"
#include "timer.hpp"

inline auto sqr(auto val)
{
    return val * val;
}

inline double to_double(long long x)
{
    return double(x);
}

template <typename T, typename Abi>
inline auto to_double(std::experimental::simd<T, Abi> const& x)
{
    return std::experimental::static_simd_cast<double>(x);
}

int main(int argc, char* argv[])
{
    long long N = 1'000'000'000;    // really large number
    int num_blocks = 4;

    if (argc > 1)
        N = std::stol(argv[1]);

    double h = 1.0 / N;
    timer t;

    t.start();

    double pi = hpx::transform_reduce(hpx::execution::par_simd,
        hpx::util::counting_iterator(0ll), hpx::util::counting_iterator(N), 0.0,
        std::plus<>{}, [&](auto i) { return h * 4.0 / (1 + sqr(to_double(i) * h)); });

    t.stop();

    std::println("pi: {}, time: {} [ms]", pi, t.elapsed());

    return 0;
}
