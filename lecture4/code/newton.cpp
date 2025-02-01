
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
        << "*********************\n"
        << "Newton Raphson Method\n"
        << "*********************\n";

    unsigned int step = 1;
    while (step != max_iterations)
    {
        double fn = f(xn);
        if (fabs(fn) <= epsilon)
        {
            // found approximation of root
            std::cout << "\nRoot is: " << xn << "\n";
            break;
        }

        double gn = f_prime(xn);
        if (gn == 0.0)  // Derivative is equal to zero, can't use this method
        {
            std::cout << "Derivative is equal to zero, can't use this method.";
            exit(0);
        }

        std::cout
            << "Iteration-" << step << ": "
            << " x = " << std::setw(10) << xn << " and"
            << " f(x) = " << std::setw(10) << fn << "\n";

        xn = xn - fn / gn;

        if (++step == max_iterations)
        {
            std::cout << "Method diverges, couldn't find root\n";
        }
    }
    return 0;
}