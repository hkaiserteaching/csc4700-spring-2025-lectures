project(spring2025-csc4700-fork-join)

set(CMAKE_CXX_STANDARD 23)

# make HPX available
find_package(HPX REQUIRED)  

add_executable(hello_hpx code/hello_hpx.cpp)

# hello_hpx depends on HPX
target_link_libraries(hello_hpx HPX::hpx)

add_executable(query_environment code/query_environment.cpp)
target_link_libraries(query_environment HPX::hpx HPX::wrap_main)
