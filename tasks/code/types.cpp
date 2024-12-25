#include <limits>
#include <print>

template <typename T>
void output_info()
{
    std::println("{}\t{}\t{}\t{}\t{}", typeid(T).name(), sizeof(T),
        8 * sizeof(T), (std::numeric_limits<T>::min)(),
        (std::numeric_limits<T>::max)());
}

int main()
{
    std::println("Type\tBytes\tBits\tMin\tMax");
    output_info<bool>();
    output_info<short>();
    output_info<unsigned short>();
    output_info<int>();
    output_info<unsigned int>();
    output_info<long>();
    output_info<unsigned long>();
    output_info<long long>();
    output_info<unsigned long long>();
    output_info<float>();
    output_info<double>();
}