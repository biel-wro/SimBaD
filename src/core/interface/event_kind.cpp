#include "event_kind.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

BEGIN_NAMESPACE_CORE
EVENT_KIND to_event_kind(std::uint32_t num)
{
  return static_cast<EVENT_KIND>(num);
}

std::uint32_t to_numeric(EVENT_KIND ek)
{
  return static_cast<std::uint32_t>(ek);
}
END_NAMESPACE_CORE

namespace std
{
std::ostream &operator<<(std::ostream &o, simbad::core::EVENT_KIND ek)
{
  using simbad::core::EVENT_KIND;
  switch(ek)
  {
  case EVENT_KIND::NONE: return o << "NONE";
  case EVENT_KIND::CREATED: return o << "CREATED";
  case EVENT_KIND::REMOVED: return o << "REMOVED";
  case EVENT_KIND::MODIFIED: return o << "MODIFIED";
  case EVENT_KIND::TRANSFORMED: return o << "TRANSFORMED";
  case EVENT_KIND::JUMPED_OUT: return o << "JUMPED_OUT";
  case EVENT_KIND::JUMPED_IN: return o << "JUMPED_IN";
  }
  throw std::logic_error("unexpected enum value of `" +
                         std::to_string(to_numeric(ek)) + "`");
}
}
