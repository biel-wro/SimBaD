#ifndef CORE_EVENT_SOURCE_HPP
#define CORE_EVENT_SOURCE_HPP
#include "core_def.hpp"

#include "event.hpp"

#include <functional>

BEGIN_NAMESPACE_CORE
class event_source
{
public:
  using event_visitor = std::function<void(event const &)>;
  virtual void generate_events(event_visitor, size_t) = 0;
  virtual ~event_source(){}
};

END_NAMESPACE_CORE

#endif // EVENT_SOURCE_HPP
