#ifndef MOST_DISTANT_BIRTH_FILTER_HPP
#define MOST_DISTANT_BIRTH_FILTER_HPP

#include <cmath>
#include <functional>
#include <limits>
#include <simple_event.hpp>

#include "event_kind.hpp"
namespace simbad
{
namespace models
{

template <class event_type>
class most_distant_birth_filter
{
  public:
    using coord_type = typename event_type::coord_type;

    template <class EventSource>
    most_distant_birth_filter(EventSource &o)
        : max_dist(-std::numeric_limits<coord_type>::infinity()),
          fun([&o]() -> event_type { return o.next_event(); })
    {
    }
    event_type next_event()
    {
        event_type e;
        coord_type dist;
        do
        {
            e = fun();
            dist = std::fabs(e.coordinate(0));

        } while ( dist < max_dist ||
                 e.event_kind() != core::EVENT_KIND::BIRTH);
        max_dist = dist;
        return e;
    }

  private:
    coord_type max_dist;
    std::function<event_type(void)> fun;
};
}
}
#endif // MOST_DISTANT_BIRTH_FILTER_HPP
