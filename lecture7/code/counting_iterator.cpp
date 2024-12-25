
#include <algorithm>
#include <concepts>
#include <iterator>
#include <iostream>
#include <vector>

template <std::input_iterator InIt, std::output_iterator OutIt>
void copy(InIt begin, InIt end, OutIt out)
{
    while (begin != end)
        *out++ = *begin++;
}

class counting_iterator
{
    std::size_t pos;

public:
    explicit counting_iterator(std::size_t start_at = 0)
      : pos(start_at)
    {
    }

    std::size_t& operator*()
    {
        return pos;
    }    // dereference

    // prefix++
    counting_iterator& operator++()
    {
        ++pos;
        return *this;
    }

    // postfix++
    counting_iterator operator++(int)
    {
        return counting_iterator(pos++);
    }

    friend bool operator==(
        counting_iterator const& lhs, counting_iterator const& rhs)
    {
        return lhs.pos == rhs.pos;
    }
};

int main()
{
    // will print: 0 1 2 3 4 5 6 7 8 9
    std::for_each(counting_iterator(0), counting_iterator(10),
        [](std::size_t val) { std::cout << val << " "; });

    std::vector<int> src = {1, 2, 3, 4, 5};
    std::vector<int> dest;
    std::ranges::copy(src, std::back_inserter(dest));
}


