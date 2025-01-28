#include <cmath>
#include <vector>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main()
{
    int n = 5000;    // 5000 data points
    std::vector<double> x(n), y(n), z(n), w(n, 2);
    for (int i = 0; i < n; ++i)
    {
        x[i] = i * i;
        y[i] = std::sin(2 * std::acos(1) * i / 360.0);
        z[i] = std::log(i);
    }
    plt::figure();      // declare a new figure (optional if only one is used)
    plt::plot(x, y);    // automatic coloring: blue
    plt::plot(x, w, "r--");                    // red dashed line
    plt::plot(x, z, {{"label", "log(x)"}});    // legend label "log(x)"
    plt::xlim(0, 1000 * 1000);                 // x-axis interval: [0, 1e6]
    plt::title("Standard usage");              // set a title
    plt::legend();                             // enable the legend
    plt::save("standard.png");                 // save the figure

    return 0;
}