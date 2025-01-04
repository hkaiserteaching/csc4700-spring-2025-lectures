
#include <print>

#include <hpx/hpx_main.hpp>
#include <hpx/include/runtime.hpp>

int main(int argc, char* argv[])
{
    auto num_threads = hpx::get_num_worker_threads();
    auto num_localities = hpx::get_num_localities(hpx::launch::sync);
    std::println("Number of threads: {}", num_threads);
    std::println("Number of localities: {}", num_localities);
    
    auto this_thread = hpx::get_worker_thread_num();
    std::println("Sequence number of current thread: {}", this_thread);

    return 0;
}
