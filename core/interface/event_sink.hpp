#ifndef EVENT_SINK_HPP
#define EVENT_SINK_HPP

#include "core_fwd.hpp"

#include "event.hpp"

#include <functional>

BEGIN_NAMESPACE_CORE
class event_sink
{
public:
  virtual ~event_sink();
  virtual void process_event(event const &e) = 0;
};
END_NAMESPACE_CORE
#endif // EVENT_SINK_HPP