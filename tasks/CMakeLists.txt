cmake_minimum_required(VERSION 3.20)
project(spring2025-csc4700-tasks)

set(CMAKE_CXX_STANDARD 23)

find_package(HPX REQUIRED)

add_executable(pi_threads code/pi_threads.cpp)
add_executable(pi_async code/pi_async.cpp)
add_executable(types code/types.cpp)

add_executable(pi_fork_join code/pi_fork_join.cpp)
target_link_libraries(pi_fork_join HPX::hpx HPX::wrap_main)

add_executable(pi_fork_join_simd code/pi_fork_join_simd.cpp)
target_link_libraries(pi_fork_join_simd HPX::hpx HPX::wrap_main)
