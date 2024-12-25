project(spring2025-csc4700-n-body)

set(CMAKE_CXX_STANDARD 23)

find_package(blaze REQUIRED)

add_executable(naive_n_body code/naive_n_body.cpp code/EasyBMP/EasyBMP.cpp)
target_link_libraries(naive_n_body INTERFACE blaze::blaze)
