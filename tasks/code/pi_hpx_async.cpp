
#include <print>
#include <string>
#include <vector>

#include <hpx/algorithm.hpp>
#include <hpx/hpx_main.hpp>

#include "timer.hpp"

double sqr(double val)
{
    return val * val;
}

double pi_helper(long long begin, long long end, double h)
{
    double local_pi = 0.0;
    for (long long i = begin; i != end; ++i)
        local_pi += h * 4.0 / (1 + sqr(i * h));
    return local_pi;
}

int main(int argc, char* argv[])
{
    long long N = 1'000'000'000;    // really large number
    int num_blocks = hpx::get_num_worker_threads();

    if (argc > 1)
        N = std::stol(argv[1]);

    timer t;

    t.start();

    double h = 1.0 / N;
    std::vector<hpx::future<double>> part_pi;
    double pi = 0.0;
    long long block_size = N / num_blocks;

    for (int n = 0; n != num_blocks; ++n)
        part_pi.push_back(hpx::async(hpx::launch::async, pi_helper,
            n * block_size, (n + 1) * block_size, h));

    for (int n = 0; n != num_blocks; ++n)
        pi += part_pi[n].get();

    t.stop();

    std::println("pi: {}, time: {} [ms]", pi, t.elapsed());

    return 0;
}
