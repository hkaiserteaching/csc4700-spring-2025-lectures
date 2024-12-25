// Suppose you have a 1x1 square. If two points are randomly picked
// within the square, what is the expected value (average) of the
// distance between them?

#include <cmath>
#include <print>
#include <random>

// Initialize random number generator
std::random_device rd;
std::mt19937 gen(rd());

double get_random_number()
{
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(gen);
}

double sqr(double x)
{
    return x * x;
}

int main()
{
    std::println("attempts,average length,error");

    double exact_answer = (2.0 + std::sqrt(2.0) + 5 * asinh(1.0)) / 15.0;

    for (int max_attempts = 1; max_attempts < 30; ++max_attempts)
    {
        int const n_total = 1 << max_attempts;

        double accumulated_length = 0.0;
        for (int n = 0; n < n_total; ++n)
        {
            double x1 = get_random_number();
            double x2 = get_random_number();
            double y1 = get_random_number();
            double y2 = get_random_number();
            accumulated_length += std::sqrt(sqr(x2 - x1) + sqr(y2 - y1));
        }

        double length = accumulated_length / n_total;
        std::println("{},{},{}", n_total, length,
            std::fabs(length - exact_answer) / exact_answer);
    }
}