// C++ Program to illustrate the use of Condition Variables
#include <condition_variable>
#include <mutex>
#include <print>
#include <thread>

// mutex to block threads
std::mutex mtx;
std::condition_variable cv;

// Function to avoid spurious wakeup
bool data_ready = false;

// Data to be produced
int data = 0;

// Producer function working as sender
void producer()
{
    // Simulate data production
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // lock release
    std::lock_guard<std::mutex> lock(mtx);

    data = 42;            // set data
    data_ready = true;    // variable to avoid spurious wakeup

    // logging notification to console
    std::print("Data Produced: {}!\n", data);

    // notify consumer when done
    cv.notify_one();
}

// consumer that will consume what producer has produced
// working as receiver
void consumer()
{
    // locking
    std::unique_lock<std::mutex> lock(mtx);

    // waiting
    cv.wait(lock, [] { return data_ready; });

    std::print("Data consumed: {}!\n", data);
}

// drive code
int main()
{
    std::thread consumer_thread(consumer);
    std::thread producer_thread(producer);

    consumer_thread.join();
    producer_thread.join();

    return 0;
}
