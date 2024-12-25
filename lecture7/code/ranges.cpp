#include <algorithm>
#include <iterator>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

void double_even_numbers()
{
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    std::vector<int> even_numbers;
    std::copy_if(begin(numbers), end(numbers), std::back_inserter(even_numbers),
        [](int n) { return n % 2 == 0; });

    std::vector<int> results;
    std::transform(begin(even_numbers), end(even_numbers),
        std::back_inserter(results), [](int n) { return n * 2; });

    std::print("doubled even numbers (iterators): ");
    for (int n : results)
        std::print("{} ", n);
    std::println();
}

void double_even_numbers_ranges()
{
    auto results = std::views::iota(1) 
      | std::views::take(5) 
      | std::views::filter([](int n) { return n % 2 == 0; }) 
      | std::views::transform([](int n) { return n * 2; });

    std::print("doubled even numbers (ranges): ");
    for (int n : results)
        std::print("{} ", n);
    std::println();
}

int main()
{
    double_even_numbers();
    double_even_numbers_ranges();
}
