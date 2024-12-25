#include <mutex>
#include <print>
#include <string>
#include <thread>
#include <vector>

#include "timer.hpp"

double sqr(double val)
{
    return val * val;
}

double pi = 0.0;
std::mutex mtx;

void pi_helper(long long begin, long long end, double h)
{
    double local_pi = 0.0;
    for (long long i = begin; i != end; ++i)
        local_pi += h * 4.0 / (1 + sqr(i * h));

    mtx.lock();
    pi += local_pi;
    mtx.unlock();
}

int main(int argc, char* argv[])
{
    long long N = 1'000'000'000;    // really large number
    double h = 1.0 / N;
    int num_blocks = 4;

    if (argc > 1)
        N = std::stol(argv[1]);
    if (argc > 2)
        num_blocks = std::stoi(argv[2]);

    timer t;

    t.start();

    std::vector<std::thread> threads;
    long long block_size = N / num_blocks;

    for (int n = 0; n != num_blocks; ++n)
        threads.push_back(
            std::thread(pi_helper, n * block_size, (n + 1) * block_size, h));

    for (int n = 0; n != num_blocks; ++n)
        threads[n].join();

    t.stop();

    std::println("pi: {}, time: {} [ms]", pi, t.elapsed());
}
