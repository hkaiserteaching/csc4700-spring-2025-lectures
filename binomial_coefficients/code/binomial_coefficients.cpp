
#include <algorithm>
#include <concepts>
#include <functional>
#include <iterator>
#include <numeric>
#include <print>
#include <vector>

template <std::input_iterator I, std::output_iterator<std::iter_value_t<I>> O>
O add_adjacent_pairs(I first, I last, O result)
{
    using value_type = std::iter_value_t<I>;
    value_type old_value = 0;
    while (first != last)
    {
        value_type new_value = *first++;
        *result++ = new_value + old_value;
        old_value = new_value;
    }
    return result;
}

void naive_implementation()
{
    std::vector<int> binomial_coefficients = {1};
    for (int i = 1; i != 10; ++i)
    {
        for (int coeff : binomial_coefficients)
            std::print("{} ", coeff);
        std::println();

        std::vector<int> next_binomial_coefficients(
            binomial_coefficients.size() + 1);
        add_adjacent_pairs(binomial_coefficients.begin(),
            binomial_coefficients.end(), next_binomial_coefficients.begin());
        next_binomial_coefficients.back() = 1;
        std::swap(binomial_coefficients, next_binomial_coefficients);
    }
}

void use_adjacent_difference()
{
    std::vector<int> binomial_coefficients = {1};
    for (int i = 1; i != 10; ++i)
    {
        for (int coeff : binomial_coefficients)
            std::print("{} ", coeff);
        std::println();

        std::vector<int> next_binomial_coefficients(
            binomial_coefficients.size() + 1);
        std::adjacent_difference(binomial_coefficients.begin(),
            binomial_coefficients.end(), next_binomial_coefficients.begin(),
            std::plus{});
        next_binomial_coefficients.back() = 1;
        std::swap(binomial_coefficients, next_binomial_coefficients);
    }
}

int main()
{
    naive_implementation();
    use_adjacent_difference();
}