
#include <cstdlib>
#include <new>
#include <vector>

#include <blaze/Math.hpp>

constexpr int N = 10;
constexpr int M = 10;

int vectors()
{
    // Not dynamically sizable*
    double x[N];

    // memory management hell
    double* x = (double*) malloc(N * sizeof(double));

    // Limited to interface of vector<double> (not a vector)
    std::vector<double> x(N);

    // Just right, has proper interface and implementation
    blaze::DynamicVector<double> x(N);
}

int matrices()
{
    // Not dynamically sizable*
    double a[N][M];

    // Really easy to get bad performance
    double** x = ? ? ;

    // Not a matrix (or a 2D array for that matter) at all
    std::vector<std::vector<double>> x(N);

    // Just right, has proper interface and implementation
    blaze::DynamicMatrix<double> x(N, M);
}

std::vector<double> sinx_interleaved(
    size_t N, size_t terms, std::vector<double> const& x)
{
    std::vector<double> results;
    results.resize(N);
    std::for_each(std::execution::par, counting_iterator(0),
        counting_iterator(N / num_treads), [&](size_t idx) {
            double res = 0;
            for (long i = idx * num_treads; i < (idx + 1) * num_treads; ++i)
                res +=
                    std::pow(-1, i - 1) * std::pow(x, 2 * i) / factorial(2 * i);
            results[idx] = res;
        });
    return results;
}

std::vector<double> sinx_interleaved(
    size_t N, size_t terms, std::vector<double> const& x)
{
    std::vector<double> results;
    results.resize(N);
    hpx::execution::experimental::dynamic_chunk_size dcs;
    hpx::for_each(hpx::execution::par.with(dcs), counting_iterator(0),
        counting_iterator(N / num_treads), [&](size_t idx) {
            // ...
        });
    return results;
}

template <typename T, typename Const, typename Count>
Count mandelbrot(T z, Const const c, Count count)
{
    size_t const max_iterations = 100;
    auto msk = abs(z) > 2.0f;
    for (size_t k = 0; k < iterations; ++k)
    {
        z = z * z + c;
        auto curr_msk = abs(z) > 2.0f;
        stdx::simd_select(msk ^ curr_msk, count) = k;
        if (std::all_of(curr_msk))
            return count;
        msk = curr_msk;
    }
    return {-1};
}

size_t mandelbrot(
    std::complex<float>(0.0, 0.0), std::complex<float>(c), size_t count);

stdx::simd<size_t> mandelbrot(stdx::simd<std::complex<float>>(0.0, 0.0),
    std::complex<float>(c), stdx::simd<size_t> count);

void mandel()
{
    hpx::for_loop(policy1, 0, height, [=, &image](int i) {
        auto width_begin = image.begin() + width * i;
        hpx::for_each(
            policy2, width_begin, width_begin + width, [=, &image](auto& j) {
                using Vector = std::decay_t<decltype(j)>;
                using Mask = get_mask_type<Vector>::type;

                const Vector x = x_min + (j) *dx;
                const Vector y = y_min + (i) *dy;
                const Vector four(4), two(2), iters(iterations), rgb(255);

                Vector count = 0;
                Mask msk(0);

                Vector zr = x;
                Vector zi = y;
                for (float k = 0; k < iterations; k++)
                {
                    Vector r2 = zr * zr;
                    Vector i2 = zi * zi;
                    Mask curr_msk = (r2 + i2) > four;

                    if (all_of_simd(curr_msk))
                    {
                        mask_assign(msk ^ curr_msk, count, k);
                        break;
                    }
                    mask_assign(msk ^ curr_msk, count, k);
                    msk = curr_msk;
                    zi = two * zr * zi + y;
                    zr = r2 - i2 + x;
                }
                count /= iters;
                count *= rgb;
                j = count;
            });
    });
}

void mandel1()
{
    BMP mandelbrot_img;
    mandelbrot_img.SetBitDepth(24);
    mandelbrot_img.SetSize(size_x, size_y);

    std::simd<float> init_pixels({0.f, 1.f, 2.f, 3.f});
    std::for_each(hpx::execution::par,
        counting_iterator(0), counting_iterator(size_y), 
        [&](size_t pixel_y) {
            std::simd<float> imag = 4.0 * pixel_y / size_y - 2.0;

            hpx::for_each(hpx::execution::experimental::simd, 
                width_begin, width_begin + width, [=, &image](auto &j)
            {
                std::simd<float> real = init_pixels + 4.0 * j / size_x - 2.0;
                auto values = mandelbrot(std::simd<std::complex>(),
                    std::simd<std::complex>({real, imag}), std::simd<size_t>());
                write_pixels(mandelbrot_img, values);
            }
        });
}

