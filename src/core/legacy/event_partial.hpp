#ifndef EVENT_PARTIAL_HPP
#define EVENT_PARTIAL_HPP

#include "interface/interface_fwd.hpp"

BEGIN_NAMESPACE_CORE

class event_partial
{
public:
  virtual EVENT_KIND event_kind() const = 0;
  virtual std::size_t dimension() const = 0;
  virtual double coordinate(std::size_t d) const = 0;
  virtual ~event_partial();
};

END_NAMESPACE_CORE
#endif // EVENT_PARTIAL_HPP
