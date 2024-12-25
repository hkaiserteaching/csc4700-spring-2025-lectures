project(spring2025-csc4700-lecture8)

set(CMAKE_CXX_STANDARD 23)

find_package(blaze REQUIRED)

add_executable(blaze_example code/blaze_example.cpp)
target_link_libraries(blaze_example INTERFACE blaze::blaze)

add_executable(blaze_matrix code/blaze_matrix.cpp)
target_link_libraries(blaze_matrix INTERFACE blaze::blaze)

add_executable(blaze_conjugate_gradient code/blaze_conjugate_gradient.cpp)
target_link_libraries(blaze_conjugate_gradient INTERFACE blaze::blaze)