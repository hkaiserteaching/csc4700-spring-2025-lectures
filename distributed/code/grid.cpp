
#include <hpx/hpx.hpp>
#include <hpx/hpx_main.hpp>

#include <blaze/Math.h>

#include <cassert>
#include <cmath>
#include <print>
#include <utility>

#include "blaze_serialization.hpp"
#include "timer.hpp"

inline double sqr(double val)
{
    return val * val;
}

using grid = blaze::DynamicMatrix<double, blaze::rowMajor>;

double jacobi_step(grid const& x, grid& y)
{
    assert(x.rows() == y.rows() && x.columns() == y.columns());
    double rnorm = 0.0;

    for (size_t i = 1; i != x.rows() - 1; ++i)
    {
        for (size_t j = 1; j != y.columns() - 1; ++j)
        {
            y(i, j) =
                (x(i - 1, j) + x(i + 1, j) + x(i, j - 1) + x(i, j + 1)) / 4.0;
            rnorm += sqr(y(i, j) - x(i, j));
        }
    }

    return std::sqrt(rnorm);
}

int jacobi(
    grid& x0, grid& x1, size_t max_iterations, double epsilon, bool debug)
{
    for (size_t step = 0; step != max_iterations; ++step)
    {
        double rnorm = jacobi_step(x0, x1);
        if (rnorm < epsilon)
            return step;
        std::swap(x0, x1);

        if (debug)
            std::print("{}: ||r|| = {}", step, rnorm);
    }
    return max_iterations;
}

double jacobi_step_parallel(grid& x, grid& y)
{
    assert(x.columns() == y.columns());

    // trigger boundary exchange
    std::uint32_t num_localities = hpx::get_num_localities(hpx::launch::sync);
    std::uint32_t locality_id = hpx::get_locality_id();

    using namespace hpx::collectives;
    auto comm = get_world_channel_communicator();

    // Ghost cell (halo) update goes here
    // Boundaries are row 0 and row x.num_x() - 1  <= update these
    //
    //  Northernmost rank (0)  -- don't change row 0
    //
    //  |  |  |  |  |  |  |  |  |  |  row 0       fill in from neighbor to north num_rows() - 2
    //  |  |  |  |  |  |  |  |  |  |  row 1        send to neighbor to north
    //  |  |  |  |  |  |  |  |  |  |  row 2
    //  ...
    //  |  |  |  |  |  |  |  |  |  |  row num_rows() - 3
    //  |  |  |  |  |  |  |  |  |  |  row num_rows() - 2
    //  |  |  |  |  |  |  |  |  |  |  row num_rows() - 1     fill in this boundary from row 1 from neighbor south
    //
    //  Southernmost rank (size-1)  -- don't change row num_rows()-1
    //
    using ghost_cells = blaze::DynamicVector<double, blaze::rowVector>;

    std::vector<hpx::future<void>> ghost_updates;
    if (locality_id != 0)
    {
        // receive ghost cells from partition above
        hpx::future<ghost_cells> recv =
            get<ghost_cells>(comm, that_site_arg(locality_id - 1), tag_arg(1));
        ghost_updates.emplace_back(recv.then(
            [&](hpx::future<ghost_cells>&& f) { row(x, 0) = f.get(); }));

        // send our ghost cells to partition above
        ghost_updates.emplace_back(set(comm, that_site_arg(locality_id - 1),
            ghost_cells(row(x, 1)), tag_arg(2)));
    }

    if (locality_id != num_localities - 1)
    {
        // receive ghost cells from partition below
        hpx::future<ghost_cells> recv =
            get<ghost_cells>(comm, that_site_arg(locality_id + 1), tag_arg(2));
        ghost_updates.emplace_back(recv.then([&](hpx::future<ghost_cells>&& f) {
            row(x, x.rows() - 1) = f.get();
        }));

        // send our ghost cells to partition below
        ghost_updates.emplace_back(set(comm, that_site_arg(locality_id + 1),
            ghost_cells(row(x, x.rows() - 2)), tag_arg(1)));
    }

    double rnorm = 0.0;

    // local stencil application
    for (size_t i = 1; i != x.rows() - 1; ++i)
    {
        for (size_t j = 1; j != y.columns() - 1; ++j)
        {
            y(i, j) =
                (x(i - 1, j) + x(i + 1, j) + x(i, j - 1) + x(i, j + 1)) / 4.0;
            rnorm += sqr(y(i, j) - x(i, j));
        }
    }

    // wait for boundary exchange to be finished
    hpx::wait_all(ghost_updates);

    // apply stencil to ghost cells
    size_t i = 1;
    for (size_t j = 1; j != x.columns() - 1; ++j)
    {
        y(i, j) = (x(i - 1, j) + x(i + 1, j) + x(i, j - 1) + x(i, j + 1)) / 4.0;
        rnorm += sqr(y(i, j) - x(i, j));
    }

    i = x.rows() - 2;
    for (size_t j = 1; j != x.columns() - 1; ++j)
    {
        y(i, j) = (x(i - 1, j) + x(i + 1, j) + x(i, j - 1) + x(i, j + 1)) / 4.0;
        rnorm += sqr(y(i, j) - x(i, j));
    }

    return std::sqrt(rnorm);
}

int jacobi_parallel(
    grid& x0, grid& x1, size_t max_iterations, double epsilon, bool debug)
{
    for (size_t step = 0; step != max_iterations; ++step)
    {
        double local_norm = jacobi_step_parallel(x0, x1);
        double rnorm = all_reduce(hpx::launch::sync,
            hpx::collectives::get_world_communicator(), local_norm,
            std::plus{});

        if (rnorm < epsilon)
            return step;
        std::swap(x0, x1);

        if (debug)
            std::print("{}: ||r|| = {}", step, rnorm);
    }
    return max_iterations;
}

void usage(std::string const& msg)
{
    std::println("Usage: {}", msg);
}

int main(int argc, char* argv[])
{
    bool seq = true;
    size_t xsize = 128, ysize = 128;
    size_t maxiter = 128;
    double tolerance = 1.e-6;
    bool debug = false;

    try
    {
        for (int arg = 1; arg < argc; ++arg)
        {
            if (std::string(argv[arg]) == "-p")
            {
                seq = false;
            }
            else if (std::string(argv[arg]) == "-x")
            {
                if (argc == ++arg)
                    usage(argv[0]);
                xsize = std::stol(argv[arg]);
            }
            else if (std::string(argv[arg]) == "-y")
            {
                if (argc == ++arg)
                    usage(argv[0]);
                ysize = std::stol(argv[arg]);
            }
            else if (std::string(argv[arg]) == "-m")
            {
                if (argc == ++arg)
                    usage(argv[0]);
                maxiter = std::stol(argv[arg]);
            }
            else if (std::string(argv[arg]) == "-d")
            {
                debug = true;
            }
            else if (std::string(argv[arg]) == "-t")
            {
                if (argc == ++arg)
                    usage(argv[0]);
                tolerance = std::stod(argv[arg]);
            }
        }
    }
    catch (...)
    {
        usage(argv[0]);
        return -1;
    }

    auto print_info = [&]() {
        std::println("parallel: {}", seq ? "No" : "Yes");
        std::println("maxiter: {}", maxiter);
        std::println("xsize: {}", xsize);
        std::println("ysize: {}", ysize);
    };

    if (seq)
    {
        if (debug)
        {
            print_info();
        }

        grid X0(xsize + 2, ysize + 2), X1(xsize + 2, ysize + 2);

        for (size_t i = 0; i != ysize + 2; ++i)
        {
            X1(0, i) = X0(0, i) = ((double) i) / ((double) ysize + 1);
        }
        for (size_t j = 0; j != xsize + 2; ++j)
        {
            X1(j, ysize + 1) = X0(j, ysize + 1) =
                ((double) j) / ((double) xsize + 1);
        }

        timer t;
        t.start();

        auto num_iters = jacobi(X1, X0, maxiter, tolerance, debug);

        t.stop();
        std::println("# elapsed time [sequential]: {} [ms]", t.elapsed());
        std::println("# {} iterations", num_iters);
    }
    return 0;
}