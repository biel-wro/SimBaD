#ifndef EVENT_1D_HPP
#define EVENT_1D_HPP
#include "integrated_wave_1d.hpp"

#include "simple_event.hpp"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D

class event_1d : public core::simple_event<float, float, 1>
{
    using my_base = simbad::core::simple_event<float,float,1>;
    using my_base::simple_event;
};
END_NAMESPACE_INTEGRATED_WAVE_1D
#endif // EVENT_1D_HPP
