project(spring2025-csc4700-distributed)

set(CMAKE_CXX_STANDARD 23)

# make HPX available
find_package(HPX REQUIRED)  

add_executable(sequential code/sequential.cpp)
target_link_libraries(sequential HPX::hpx)

add_executable(distributed code/distributed.cpp)
target_compile_definitions(distributed PRIVATE HPX_HAVE_RUN_MAIN_EVERYWHERE)
target_link_libraries(distributed HPX::hpx)
