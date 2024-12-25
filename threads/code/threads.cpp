#include <mutex>
#include <print>
#include <string>
#include <thread>
#include <vector>

int shared_int = 4700;
std::mutex mtx;

void thread_func(int num_thread)
{
    std::lock_guard<std::mutex> l(mtx);
    ++shared_int;
    std::println("Thread {}, stack {}, shared: {} ({})", num_thread,
        static_cast<void*>(&num_thread), static_cast<void*>(&shared_int),
        shared_int);
}

int main(int argc, char* argv[])
{
    int num_threads = 4;    // by default we create 2 threads
    if (argc > 1)
        num_threads = std::stoi(argv[1]);

    std::vector<std::jthread> threads;
    threads.reserve(num_threads);
    for (int i = 0; i != num_threads; ++i)
    {
        threads.push_back(std::jthread(thread_func, i));
    }

    return 0;    // destructors of threads will join implicitly
}
