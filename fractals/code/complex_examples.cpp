#include <complex>
#include <iostream>

void complex_arithmetic()
{
    std::complex<double> num1(3.0, 4.0);              // 3 + 4i
    std::complex<double> num2(1.0, 2.0);              // 1 + 2i
    std::complex<double> sum = num1 + num2;           // Adding complex numbers
    std::complex<double> difference = num1 - num2;    // Subtract them
    std::complex<double> product = num1 * num2;
    std::complex<double> quotient = num1 / num2;

    // Sum: (4,6)
    std::cout << "Sum: " << sum << std::endl;
    // Difference: 2 + 2i
    std::cout << "Difference: " << difference << std::endl;
    // Product: -5 + 10i
    std::cout << "Product: " << product << std::endl;
    // Quotient: Results in 2.2 - 0.4i
    std::cout << "Quotient: " << quotient << std::endl;
}

void complex_examples()
{
    // defines the complex number: (10 + 2i)
    std::complex<double> c(10.0, 2.0);

    // prints the real part using the real function
    std::cout << "Real part: " << std::real(c) << std::endl;
    std::cout << "Imaginary part: " << std::imag(c) << std::endl;

    // prints the absolute value of the complex number
    std::cout << "The absolute value of " << c << " is: ";
    std::cout << std::abs(c) << std::endl;

    // use of norm()
    std::cout << "The norm of " << c << " is " << std::norm(c) << std::endl;

    // prints the argument of the complex number
    std::cout << "The argument of " << c << " is: ";
    std::cout << std::arg(c) << std::endl;

    // use of polar()
    std::cout << "The complex whose magnitude is " << 2.0;
    std::cout << " and phase angle is " << 0.5;
    std::cout << " is " << std::polar(2.0, 0.5) << std::endl;
}

void complex_trigonometric_examples()
{
    // initializing the complex: (-1.0+0.0i)
    std::complex<double> c(0.0, 1.0);

    // use of cos()
    std::cout << "The cos of " << c << " is " << std::cos(c) << std::endl;

    // use of sin()
    std::cout << "The sin of " << c << " is " << std::sin(c) << std::endl;

    // use of tan()
    std::cout << "The tan of " << c << " is " << std::tan(c) << std::endl;
}

int main()
{
    complex_arithmetic();
    complex_examples();
    complex_trigonometric_examples();

    return 0;
}