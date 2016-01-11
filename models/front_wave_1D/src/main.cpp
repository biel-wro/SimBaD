#include <iostream>
#include <tuple>
#include <utility>

#include "event_kind.hpp"
#include "front_wave_1d_impl.hpp"
#include "simple_event.hpp"

int main()
{
    simbad::models::front_wave_1d_impl impl;
    size_t niters = 100;

    for (size_t iter = 0; iter < niters; ++iter)
    {
        std::pair<float, simbad::core::EVENT_KIND> event = impl.next_event();
        std::cout << event.first << " " << event.second << std::endl;
    }
    return 0;
}
