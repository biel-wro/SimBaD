#include <cstdlib>
#include <iostream>
#include <tuple>
#include <utility>

#include "event_kind.hpp"
#include "front_wave_1d_impl.hpp"
#include "most_distant_birth_filter.hpp"
#include "saturation_distance_computer.hpp"
#include "simple_event_schedule.hpp"

USING_NAMEPSPACE_FRONT_WAVE_1D

void print_configuration(front_wave_1d_impl const &impl)
{
    std::cout << "conf ";
    for (particle_1D const &p : impl)
    {
        std::cout << "(" << p.coordinate(0) << " "
                  << p.get_event_acc().get_density() << " "
                  << (*p.get_handle()).get_time() << ") ";
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{

    using impl_type = front_wave_1d_impl;

    using event_type = impl_type::Event;
    // using filter_type =
    // simbad::models::most_distant_birth_filter<event_type>;
    // using saturation_distance = simbad::models::saturation_distance_computer;

    size_t niters = 10;

    double alpha = 4.0;
    if( argc >= 2)
        alpha = atof(argv[1]);

    impl_type impl(alpha);

    if (argc >= 3)
        niters = atoi(argv[2]);

    if (argc >= 4)
    {
        uint32_t s = atoi(argv[3]);
        impl.seed(s);
    }

    // filter_type filter(impl);

    for (size_t iter = 0; iter < niters; ++iter)
    {
        event_type event = impl.next_event();
        std::cout << event.time() << " " << event.coordinate(0);

        //double sat_dist = saturation_distance().observe(impl);

        std::cout << std::endl;
    }
    //print_configuration(impl);

    return 0;
}
