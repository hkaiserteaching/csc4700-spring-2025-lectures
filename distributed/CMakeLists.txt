project(spring2025-csc4700-distributed)

set(CMAKE_CXX_STANDARD 23)

find_package(blaze REQUIRED)    # make Blaze available
find_package(HPX REQUIRED)      # make HPX available

add_executable(sequential code/sequential.cpp)
target_link_libraries(sequential HPX::hpx)

add_executable(hello_world code/hello_world.cpp)
target_compile_definitions(hello_world PRIVATE HPX_HAVE_RUN_MAIN_EVERYWHERE)
target_link_libraries(hello_world HPX::hpx)

add_executable(distributed code/distributed.cpp)
target_compile_definitions(distributed PRIVATE HPX_HAVE_RUN_MAIN_EVERYWHERE)
target_link_libraries(distributed HPX::hpx)

add_executable(four_hpx_functions_p2p code/four_hpx_functions_p2p.cpp)
target_compile_definitions(four_hpx_functions_p2p PRIVATE HPX_HAVE_RUN_MAIN_EVERYWHERE)
target_link_libraries(four_hpx_functions_p2p HPX::hpx)

add_executable(grid code/grid.cpp)
target_compile_definitions(grid PRIVATE HPX_HAVE_RUN_MAIN_EVERYWHERE NOMINMAX)
target_link_libraries(grid HPX::hpx blaze::blaze)
