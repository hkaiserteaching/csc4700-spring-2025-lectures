project(spring2025-csc4700-lecture4)

set(CMAKE_CXX_STANDARD 23)

find_package(Boost REQUIRED)

add_executable(fixed_point code/fixed_point.cpp)
add_executable(fixed_point_iteration_cond code/fixed_point_iteration_cond.cpp)

add_executable(fixed_point_iteration code/fixed_point_iteration.cpp)
target_link_libraries(fixed_point_iteration Boost::boost)

add_executable(square_root code/square_root.cpp)
target_link_libraries(square_root Boost::boost)

add_executable(square_root1 code/square_root1.cpp)

add_executable(bisection code/bisection.cpp)
add_executable(bisection_fixed_point code/bisection_fixed_point.cpp)

add_executable(newton code/newton.cpp)
add_executable(newton1 code/newton1.cpp)
add_executable(newton2 code/newton2.cpp)
add_executable(newton_fixed_point code/newton_fixed_point.cpp)

add_executable(gradient_descent code/gradient_descent.cpp)
