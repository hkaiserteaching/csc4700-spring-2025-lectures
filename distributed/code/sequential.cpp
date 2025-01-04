#include <print>
#include <string>

double sqr(double val)
{
    return val * val;
}

int main(int argc, char* argv[])
{
    long N = 1'000'000'000;
    if (argc > 1)
        N = std::stol(argv[1]);
    double h = 1.0 / N;

    double pi = 0.0;
    for (long i = 0; i != N / 4; ++i)
        pi += h * 4.0 / (1 + sqr(i * h));

    std::println("pi: {}", pi);
}
