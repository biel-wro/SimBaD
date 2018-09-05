#ifndef CORE_EVENT_KIND_HPP
#define CORE_EVENT_KIND_HPP

#include "interface/interface_fwd.hpp"

#include <iosfwd>

BEGIN_NAMESPACE_CORE

enum class EVENT_KIND : std::uint32_t
{                  // position  | ID         | attributes
                   // ----------+------------+-----------
  NONE = 0,        // preserved | preserved  | preserved
  CREATED = 1,     // assigned  | assigned   | assigned
  REMOVED = 2,     // freed     | freed      | undefined
  MODIFIED = 3,    // preserved | preserved  | changed
  TRANSFORMED = 4, // preserved | reassigned | changed
  JUMPED_OUT = 5,  // freed     | saved      | saved
  JUMPED_IN = 6,   // assigned  | restored   | restored
};

EVENT_KIND to_event_kind(std::uint32_t num);
std::uint32_t to_numeric(EVENT_KIND ek);

END_NAMESPACE_CORE

namespace std {
std::ostream &operator<<(std::ostream &o, simbad::core::EVENT_KIND ek);
}


#endif
