
#include <iostream>
#include <iomanip>
#include <cmath>

// Defining equation to be solved.
// Change this equation to solve another problem.
double f(double x)
{
    return 3 * x - std::cos(x) - 1;
}

// Calculate slope of f(x) at x.
double f_prime(double f(double), double x, double epsilon)
{
    double delta = epsilon / 50;
    return (f(x + delta) - f(x - delta)) / (2 * delta);
}

double newton(double f(double), double xn,
              unsigned int max_iterations = 100, double epsilon = 0.0001)
{
    unsigned int step = 1;
    while (step != max_iterations)
    {
        double fn = f(xn);
        if (fabs(fn) <= epsilon)
        {
            break; // found approximation of root
        }

        double gn = f_prime(f, xn, epsilon);
        if (gn == 0.0)
        {
            throw std::runtime_error("Derivative is equal to zero, can't use this method.");
        }

        xn = xn - fn / gn;

        if (++step == max_iterations)
        {
            throw std::runtime_error("Method diverges, couldn't find root");
        }
    }
    return xn;
}

int main()
{
    // Inputs
    double xn;
    std::cout << "Enter initial guess: ";
    std::cin >> xn;

    double epsilon;
    std::cout << "Enter tolerable error: ";
    std::cin >> epsilon;

    unsigned int max_iterations;
    std::cout << "Enter maximum number of iterations: ";
    std::cin >> max_iterations;

    std::cout << "\n"
        << "*********************\n"
        << "Newton Raphson Method\n"
        << "*********************\n";

    try
    {
        double root = newton(f, xn, max_iterations, epsilon);
        std::cout << "\nRoot is: " << root << "\n";
    }
    catch (std::runtime_error const &e)
    {
        std::cout << e.what() << "\n";
    }

    return 0;
}