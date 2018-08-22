#ifndef EVENT_SINK_HPP
#define EVENT_SINK_HPP

#include "interface/interface_fwd.hpp"

#include <functional>

BEGIN_NAMESPACE_CORE
class event_sink
{
public:
  virtual ~event_sink();
  virtual void set_description(attribute_description const &) = 0;
  virtual void read_event(std::size_t events_since_last_read,
                          event const &) = 0;

protected:
};
END_NAMESPACE_CORE
#endif // EVENT_SINK_HPP
