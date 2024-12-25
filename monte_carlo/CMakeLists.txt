project(spring2025-csc4700-monte-carlo)

set(CMAKE_CXX_STANDARD 23)

find_package(HPX REQUIRED)

set(Python3_ROOT_DIR "C:/CVS/vcpkg/installed/x64-windows")
find_package(Python3 COMPONENTS Development REQUIRED)
message(Python3_LIBRARIES: ${Python3_LIBRARIES})

add_executable(rand code/rand.cpp)
add_executable(uniform_rand code/uniform_rand.cpp)
add_executable(line_length code/line_length.cpp)
add_executable(rolling_dice code/rolling_dice.cpp)
add_executable(compute_pi code/compute_pi.cpp)
add_executable(compute_pi_error code/compute_pi_error.cpp)

add_executable(compute_pi_error_parallel code/compute_pi_error_parallel.cpp)
target_link_libraries(compute_pi_error_parallel HPX::hpx)

add_executable(birthdays code/birthdays.cpp)

add_executable(birthdays_parallel code/birthdays_parallel.cpp)
target_link_libraries(birthdays_parallel HPX::hpx)

add_executable(random_numbers code/random_numbers.cpp)
target_compile_definitions(random_numbers PRIVATE WITHOUT_NUMPY)
target_link_libraries(random_numbers Python3::Python Python3::Module)
