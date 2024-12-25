#include <cstdlib>    // Include for using rand
#include <ctime>      // Include for getting the current time
#include <print>

int main()
{
    // Use the current time as random seed
    std::srand(std::time(0));

    // Get one random number
    int random_value = std::rand();

    // print result
    std::println("Random value in range [0, {}]: {}", RAND_MAX, random_value);
}
