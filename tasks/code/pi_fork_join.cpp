
#include <numeric>
#include <print>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <hpx/algorithm.hpp>
#include <hpx/hpx_main.hpp>
#include <hpx/iterator_support/counting_iterator.hpp>

//#include "counting_iterator.hpp"
#include "safe_object.hpp"
#include "timer.hpp"

inline auto sqr(auto val)
{
    return val * val;
}

int main(int argc, char* argv[])
{
    long N = 1'000'000'000;    // really large number
    int num_blocks = 4;

    if (argc > 1)
        N = std::stol(argv[1]);

    double h = 1.0 / N;
    timer t;

    t.start();

    double pi = 0.0;
    hpx::experimental::for_loop(hpx::execution::par,
        0l, N, hpx::experimental::reduction_plus(pi),
        [&](auto i, auto& local_pi)
        {
            local_pi += h * 4.0 / (1 + sqr(double(i) * h));
        }
    );

    t.stop();

    std::println("pi: {}, time: {} [ms]", pi, t.elapsed());

    return 0;
}
