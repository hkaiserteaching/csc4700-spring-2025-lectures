
#include <chrono>
#include <cmath>
#include <print>
#include <random>

// Initialize random number generator
std::random_device rd;
std::mt19937 gen(rd());

double get_random_number()
{
    std::uniform_real_distribution<double> dis(0, 1);
    return dis(gen);
}

double sqr(double val)
{
    return val * val;
}

int main()
{
    std::println("attempts,pi,error,realerror,elapsed");

    std::chrono::high_resolution_clock t;
    double real_pi = 2 * std::acos(0.0);
    for (int max_attempts = 1; max_attempts < 30; ++max_attempts)
    {
        auto start = t.now();
        int const n_total = 1 << max_attempts;

        double S_n = 0.0;
        double error_term = 0.0;

        for (int i = 0; i != n_total; ++i)
        {
            double x_i = get_random_number();
            double y = 4.0 / (1.0 + sqr(x_i));
            S_n += y;
            error_term += sqr(y);
        }

        double pi = S_n / n_total;
        double error =
            std::sqrt(std::fabs(error_term - n_total * sqr(pi))) /
            n_total;

        std::println("{},{},{},{},{:%S}", n_total, pi,
            fabs(pi - real_pi) / real_pi, error / real_pi, t.now() - start);
    }

    return 0;
}
