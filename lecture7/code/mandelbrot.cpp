#include <algorithm>
#include <complex>
#include <concepts>

#include "counting_iterator.hpp"

#include "EasyBMP/EasyBMP.h"

template <typename F, typename Cond, std::integral Iterations, typename Float>
    requires(std::same_as<std::invoke_result_t<F, Float>, Float> &&
        std::same_as<std::invoke_result_t<Cond, Float, Float>, bool>)
std::pair<Float, Iterations> fixed_point(
    F f, Cond cond, Iterations n, Float init)
{
    Float x = init;
    for (Iterations i = 1; i != n; ++i)
    {
        Float x1 = f(x);
        if (cond(x1, x))
            return {x1, i};
        x = x1;
    }
    return {x, n};
}

// Kernel to compute the Mandelbrot set
size_t mandelbrot(std::complex<double> c)
{
    size_t const max_iterations = 100;
    std::complex<double> z(0.0, 0.0);
    for (size_t i = 0; i != max_iterations; ++i)
    {
        z = z * z + c;
        if (std::abs(z) > 2.0)
            return i;
    }
    return -1;
}

// Kernel to compute the Mandelbrot set
size_t mandelbrot_fixed_point(std::complex<double> c)
{
    size_t const max_iterations = 100;
    using float_type = std::complex<double>;

    auto [_, iterations] = fixed_point(
        // function to find fixed point for
        [&](float_type z) { return z = z * z + c; },
        // termination condition
        [](float_type z, float_type) { return std::abs(z) > 2.0; },
        max_iterations, float_type(0, 0));

    return iterations == max_iterations ? -1 : iterations;
}

static RGBApixel const colors[] = {
    RGBApixel{66, 30, 15},
    RGBApixel{25, 7, 26},
    RGBApixel{9, 1, 47},
    RGBApixel{4, 4, 73},
    RGBApixel{0, 7, 100},
    RGBApixel{12, 44, 138},
    RGBApixel{24, 82, 177},
    RGBApixel{57, 125, 209},
    RGBApixel{134, 181, 229},
    RGBApixel{211, 236, 248},
    RGBApixel{241, 233, 191},
    RGBApixel{248, 201, 95},
    RGBApixel{255, 170, 0},
    RGBApixel{204, 128, 0},
    RGBApixel{153, 87, 0},
    RGBApixel{106, 52, 3},
};

RGBApixel get_rgb(int value)
{
    return colors[value % 16];
}

int main()
{
    int const size_x = 2048;
    int const size_y = size_x;

    BMP mandelbrot_img;
    mandelbrot_img.SetBitDepth(24);
    mandelbrot_img.SetSize(size_x, size_y);

    std::for_each(
        counting_iterator(0), counting_iterator(size_y), [&](size_t pixel_y) {
            double imag = 4.0 * pixel_y / size_y - 2.0;

            for (size_t pixel_x = 0; pixel_x != size_x; ++pixel_x)
            {
                // Get the number of iterations (-1 means converging)
                double real = 4.0 * pixel_x / size_x - 2.0;
                int value = mandelbrot(std::complex(real, imag));
                if (value == -1)
                {
                    // Not diverging, part of Mandelbrot Set
                    mandelbrot_img.SetPixel(
                        pixel_x, pixel_y, RGBApixel(0, 0, 0));
                }
                else
                {
                    // Convert the value to RGB color space and set the pixel color
                    mandelbrot_img.SetPixel(pixel_x, pixel_y, get_rgb(value));
                }
            }
        });

    // Save the image
    mandelbrot_img.WriteToFile("mandelbrot.bmp");

    return 0;
}
