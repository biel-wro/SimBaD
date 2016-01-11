#include "front_wave_1d_impl.hpp"

#include <tuple>
#include <utility>

#include "front_wave_1d_algorithms.hpp"

namespace simbad
{
namespace models
{
using algorithms = front_wave_1d_algorithms;

front_wave_1d_impl::front_wave_1d_impl() : t(0.0) { reinitialize(); }

front_wave_1d_impl::~front_wave_1d_impl()
{
    space.clear();
}

void front_wave_1d_impl::reinitialize()
{
    space = algorithms::initial_configuration();
    queue = algorithms::init_event_queue(rnd, space);
}

std::pair<float, front_wave_1d_impl::EVENT_KIND>
front_wave_1d_impl::next_event()
{
    EVENT_KIND event= algorithms::execute_event(t,rnd,space,queue);
    return std::make_pair(float(t),event);
}
}
}
