// Would it be profitable given 24 rolls of a pair of fair dice to bet against
// there being at least one double six?

#include <cmath>
#include <print>
#include <random>

// Initialize random number generator
std::random_device rd;
std::mt19937 gen(rd());

int roll_dice()
{
    std::uniform_int_distribution<int> dis(1, 6);
    return dis(gen);
}

bool attempt_to_win()
{
    for (int r = 0; r < 24; ++r)
    {
        int r1 = roll_dice();
        int r2 = roll_dice();
        if (r1 == 6 && r2 == 6)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    std::println("attempts,probability,error");

    double exact_answer = 1.0 - std::pow(35.0 / 36.0, 24);

    for (int max_attempts = 1; max_attempts < 30; ++max_attempts)
    {
        int const n_total = 1 << max_attempts;

        int num_wins = 0;
        for (int n = 0; n < n_total; ++n)
        {
            if (attempt_to_win())
            {
                ++num_wins;
            }
        }

        double probability_of_win = double(num_wins) / n_total;
        std::println("{},{},{}", n_total, probability_of_win,
            std::fabs(probability_of_win - exact_answer) / exact_answer);
    }
}
