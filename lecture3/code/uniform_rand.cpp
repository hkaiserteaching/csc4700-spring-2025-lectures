// Include for advanced random numbers
#include <print>
#include <random>

int main()
{
    // Generate a random number device
    std::random_device rd;

    // Use the standard mersenne_twister_engine
    std::mt19937 gen(rd());

    // Specify the interval [1, 6]
    std::uniform_int_distribution<int> dis(1, 6);

    // Specify the interval [1.0 ,6.0]
    std::uniform_real_distribution<double> disd(1, 6);

    std::println("{} {}", dis(gen), disd(gen));
}