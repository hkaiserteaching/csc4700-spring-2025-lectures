project(spring2025-csc4700-integration)

set(CMAKE_CXX_STANDARD 23)

find_package(HPX REQUIRED)

add_executable(monte_carlo_pi code/monte_carlo_pi.cpp)

add_executable(monte_carlo_pi_parallel code/monte_carlo_pi_parallel.cpp)
target_link_libraries(monte_carlo_pi_parallel HPX::hpx)