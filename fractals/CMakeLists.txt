project(spring2025-csc4700-fractals)

set(CMAKE_CXX_STANDARD 23)

find_package(HPX REQUIRED)

add_executable(complex_examples code/complex_examples.cpp)
add_executable(mandelbrot code/mandelbrot.cpp code/EasyBMP/EasyBMP.cpp)
add_executable(mandelbrot_area code/mandelbrot_area.cpp)
