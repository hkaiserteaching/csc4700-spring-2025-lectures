
#include <hpx/hpx.hpp>
#include <hpx/hpx_main.hpp>

#include <print>

int main()
{
    std::uint32_t num_localities = hpx::get_num_localities(hpx::launch::sync);
    std::uint32_t locality_id = hpx::get_locality_id();

    if (locality_id == 0)
    {
        int received = get<int>(hpx::launch::sync,
            hpx::collectives::get_world_channel_communicator(),
            hpx::collectives::that_site_arg(1));
        std::print("locality_id(0): received: {}", received);
    }
    else
    {
        set(hpx::collectives::get_world_channel_communicator(),
            hpx::collectives::that_site_arg(0), 42);
    }
    return 0;
}