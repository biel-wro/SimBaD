#ifndef EVENT_1D_HPP
#define EVENT_1D_HPP

#include "simple_event.hpp"
namespace simbad
{
namespace models
{
class event_1d : public core::simple_event<float, float, 1>
{
    using my_base = simbad::core::simple_event<float,float,1>;
    using my_base::simple_event;
};
}
}

#endif // EVENT_1D_HPP
