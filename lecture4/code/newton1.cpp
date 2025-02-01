
#include <iostream>
#include <iomanip>
#include <cmath>

// Defining equation to be solved.
// Change this equation to solve another problem.
double f(double x)
{
    return 3 * x - std::cos(x) - 1;
}

// Defining derivative of f(x).
// As you change f(x), change this function also.
double f_prime(double x)
{
    return 3 + std::sin(x);
}

double newton(double f(double), double f_prime(double), unsigned int max_iterations,
              double xn, double epsilon)
{
    unsigned int step = 1;
    while (step != max_iterations)
    {
        double fn = f(xn);
        if (fabs(fn) <= epsilon)
        {
            break; // found approximation of root
        }

        double gn = f_prime(xn);
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

    std::cout
        << "\n"
        << "*********************\n"
        << "Newton Raphson Method\n"
        << "*********************\n";

    try
    {
        double root = newton(f, f_prime, max_iterations, xn, epsilon);
        std::cout << "\nRoot is: " << root << "\n";
    }
    catch (std::runtime_error const &e)
    {
        std::cout << e.what() << "\n";
    }

    return 0;
}