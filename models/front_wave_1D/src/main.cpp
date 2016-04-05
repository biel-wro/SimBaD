#include <cstdlib>
#include <iostream>
#include <tuple>
#include <utility>

#include "event_kind.hpp"
#include "front_wave_1d_impl.hpp"
#include "most_distant_birth_filter.hpp"
#include "saturation_distance_computer.hpp"
#include "simple_event_schedule.hpp"
void print_configuration(simbad::models::front_wave_1d_impl const &impl)
{
    std::cout << "conf ";
    for (simbad::models::particle_1D const &p : impl)
    {
        std::cout << "(" << p.coordinate(0) << " "
                  << p.get_event_acc().get_density() << " "
                  << (*p.get_handle()).get_time() << ") ";
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{

    using impl_type = simbad::models::front_wave_1d_impl;
    impl_type impl(3.0);
    using event_type = impl_type::Event;
    // using filter_type =
    // simbad::models::most_distant_birth_filter<event_type>;
    using saturation_distance = simbad::models::saturation_distance_computer;

    size_t niters = 10;

    if (argc >= 2)
        niters = atoi(argv[1]);

    if (argc >= 3)
    {
        uint32_t s = atoi(argv[2]);
        impl.seed(s);
    }

    // filter_type filter(impl);

    for (size_t iter = 0; iter < niters; ++iter)
    {
        event_type event = impl.next_event();
        // std::cout << event << std::endl;
        double sat_dist = saturation_distance().observe(impl);

        std::cout << event.time() << " " << event.coordinate(0) << " "
                  << sat_dist << std::endl;
    }
    //print_configuration(impl);

    return 0;
}
