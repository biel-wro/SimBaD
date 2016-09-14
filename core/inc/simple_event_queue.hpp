#ifndef SIMPLE_EVENT_QUEUE_HPP
#define SIMPLE_EVENT_QUEUE_HPP

#include <functional>
#include "core_def.hpp"
#include "simple_event_schedule.hpp"

//#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/pairing_heap.hpp>

BEGIN_NAMESPACE_CORE

struct increasing_time_order
{
  template <class Event>
  bool operator()(Event const &lhs, Event const &rhs) const
  {
    return lhs.get_time() >= rhs.get_time();
  }
};

using simple_event_queue_base =
    boost::heap::pairing_heap<simple_event_schedule,
                              boost::heap::compare<increasing_time_order>>;

class simple_event_queue : public simple_event_queue_base
{
  using simple_event_queue_base::pairing_heap;
};

using simple_event_handle = simple_event_queue::handle_type;
END_NAMESPACE_CORE
#endif // SIMPLE_EVENT_QUEUE_HPP
