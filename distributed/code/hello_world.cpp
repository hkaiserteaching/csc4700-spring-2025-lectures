
#include <print>
#include <string>

#include <hpx/hpx.hpp>
#include <hpx/hpx_main.hpp>

int main()
{
    std::uint32_t num_localities = hpx::get_num_localities(hpx::launch::sync);
    std::uint32_t locality_id = hpx::get_locality_id();

    std::print("Hello world! ");
    std::println("I am {} of {}", locality_id, num_localities);

    return 0;
}