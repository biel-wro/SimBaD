#ifndef SIMPLE_EVENT_QUEUE_HPP
#define SIMPLE_EVENT_QUEUE_HPP

#include <functional>

#include "simple_event.hpp"

#include <boost/heap/fibonacci_heap.hpp>

namespace simbad
{
namespace core
{

struct increasing_time_order
{
    template <class Event>
    bool operator()(Event const &lhs, Event const &rhs) const
    {
        return lhs.get_time() > rhs.get_time();
    }
};

using simple_event_queue_base = boost::heap::fibonacci_heap < simple_event,
      boost::heap::compare<increasing_time_order>>;


class simple_event_queue : public simple_event_queue_base

{
  using simple_event_queue_base::fibonacci_heap;
};

using simple_event_handle = simple_event_queue::handle_type;
}
}
#endif // SIMPLE_EVENT_QUEUE_HPP
