
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
    int const n_total = 10000;

    int nc = 0;
    for (int n = 0; n < n_total; ++n)
    {
        double x = get_random_number();
        double y = get_random_number();
        if (sqr(x) + sqr(y) <= 1.0)
        {
            ++nc;
        }
    }

    std::println("Approximated pi for {} attempts: {}", n_total, (4.0 * nc) / n_total);
}