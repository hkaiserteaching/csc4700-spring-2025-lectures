#include <cstddef>
#include <cstdint>
#include <print>
#include <string>

#include <hpx/hpx.hpp>
#include <hpx/hpx_main.hpp>

inline double sqr(double val)
{
    return val * val;
}

int main(int argc, char* argv[])
{
    long N = 1'000'000'000;
    std::uint32_t num_localities = hpx::get_num_localities(hpx::launch::sync);
    std::uint32_t locality_id = hpx::get_locality_id();

    if (locality_id == 0 && argc > 1)
        N = std::stol(argv[1]);

    hpx::collectives::broadcast(hpx::collectives::get_world_communicator(), N);

    std::size_t blocksize = N / num_localities;
    std::size_t begin = blocksize * locality_id;
    std::size_t end = blocksize * (locality_id + 1);
    double h = 1.0 / N;

    double pi = 0.0;
    for (long i = begin; i != end; ++i)
        pi += h * 4.0 / (1 + sqr(i * h));

    hpx::collectives::reduce(
        hpx::collectives::get_world_communicator(), pi, std::plus{});

    if (locality_id == 0)
        std::println("pi: {}", pi);
}
