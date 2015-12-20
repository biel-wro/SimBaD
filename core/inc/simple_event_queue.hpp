#ifndef SIMPLE_EVENT_QUEUE_HPP
#define SIMPLE_EVENT_QUEUE_HPP

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

using simple_event_queue = boost::heap::fibonacci_heap<
    simple_event,                               // data type
    boost::heap::compare<increasing_time_order> // comparator
    >;

using simple_event_handle = simple_event_queue::handle_type;

}
}
#endif // SIMPLE_EVENT_QUEUE_HPP
