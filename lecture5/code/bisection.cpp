
#include <cmath>
#include <iomanip>
#include <iostream>

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
    double xn = 0, xm = 1;
//    std::cout << "Enter initial lower guess: ";
//    std::cin >> xn;

//    std::cout << "Enter initial upper guess: ";
//    std::cin >> xm;

//    std::cout << xn << ", " << xm << "\n";

    if (std::signbit(f(xn)) == std::signbit(f(xm)))
    {
        std::cout << "Lower and upper guesses should produce function values of different sign.";
        return -1;
    }

    double epsilon = 1e-6;
//    std::cout << "Enter tolerable error: ";
//    std::cin >> epsilon;

    unsigned int max_iterations = 100;
//    std::cout << "Enter maximum number of iterations: ";
//    std::cin >> max_iterations;

    std::cout << "*********************\n"
              << "Bisection Method\n"
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

        double fm = f(xm);
        if (fabs(fm) <= epsilon)
        {
            // found approximation of root
            std::cout << "\nRoot is: " << xm << "\n";
            break;
        }
 
        std::cout << "Iteration-" << step << ": "
                  << " xn = " << std::setw(10) << xn << " and"
                  << " f(xn) = " << std::setw(10) << fn << "\n"
                  << " xm = " << std::setw(10) << xm << " and"
                  << " f(xm) = " << std::setw(10) << fm << "\n";

        double delta = xn + (xm - xn) / 2;
        if (std::signbit(fn) == std::signbit(f(delta))) {
            xn = delta;
        }
        else {
            xm = delta;
        }

        if (++step == max_iterations)
        {
            std::cout << "Method diverges, couldn't find root\n";
        }
    }
    return 0;
}