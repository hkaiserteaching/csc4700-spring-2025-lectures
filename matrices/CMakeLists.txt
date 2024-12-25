project(spring2025-csc4700-matrices)

set(CMAKE_CXX_STANDARD 23)

find_package(blaze REQUIRED)

add_executable(dot_product code/dot_product.cpp)
target_link_libraries(dot_product INTERFACE blaze::blaze)

add_executable(matrix_vector_product code/matrix_vector_product.cpp)
target_link_libraries(matrix_vector_product INTERFACE blaze::blaze)