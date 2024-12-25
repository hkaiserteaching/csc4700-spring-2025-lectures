// Would it be profitable given 24 rolls of a pair of fair dice to bet against
// there being at least one double six?

#include <cmath>
#include <chrono>
#include <print>
#include <random>

// Initialize random number generator
std::random_device rd;
std::mt19937 gen(rd());

int get_birthday()
{
    std::uniform_int_distribution<int> dis(1, 365);
    return dis(gen);
}

bool have_common_birthdays()
{
    std::vector<int> birthdays(30);
    std::generate(birthdays.begin(), birthdays.end(), get_birthday);
    std::sort(birthdays.begin(), birthdays.end());
    return std::unique(birthdays.begin(), birthdays.end()) != birthdays.end();
}

int main()
{
    std::println("attempts,probability,error,elapsed");

    double exact_answer = 1.0;
    for (int d = 336; d != 365; ++d)
        exact_answer *= double(d) / 365.0;
    exact_answer = 1.0 - exact_answer;

    std::chrono::high_resolution_clock t;
    for (int max_attempts = 1; max_attempts < 30; ++max_attempts)
    {
        auto start = t.now();

        int const n_total = 1 << max_attempts;

        int num_common_birthdays = 0;
        for (int n = 0; n < n_total; ++n)
        {
            if (have_common_birthdays())
            {
                ++num_common_birthdays;
            }
        }

        double probability_of_common_birthdays =
            double(num_common_birthdays) / n_total;
        std::println("{},{},{},{}", n_total, probability_of_common_birthdays,
            std::fabs(probability_of_common_birthdays - exact_answer) /
                exact_answer, t.now() - start);
    }
}
