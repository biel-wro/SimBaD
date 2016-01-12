#include <iostream>
#include <tuple>
#include <utility>

#include "event_kind.hpp"
#include "front_wave_1d_impl.hpp"
#include "most_distant_birth_filter.hpp"
#include "simple_event_schedule.hpp"

int main()
{
    using impl_type = simbad::models::front_wave_1d_impl;
    impl_type impl;
    using event_type = impl_type::Event;
    using filter_type = simbad::models::most_distant_birth_filter<event_type>;

    size_t niters = 100;
    filter_type filter(impl);

    for (size_t iter = 0; iter < niters; ++iter)
    {
        event_type event = filter.next_event();
        //std::cout << event << std::endl;
        std::cout << event.time() << " " << event.coordinate(0) << std::endl;
    }
    return 0;
}
