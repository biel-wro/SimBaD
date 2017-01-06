#ifndef CORE_EVENT_KIND_HPP
#define CORE_EVENT_KIND_HPP

#include "core_fwd.hpp"

#include <iosfwd>

BEGIN_NAMESPACE_CORE

enum class EVENT_KIND : std::uint32_t
{              // position   | ID         | attributes
  NONE,        // preserved  | preserved  | preserved
  CREATED,     // assigned   | assigned   | assigned
  REMOVED,     // freed      | freed      | undefined
  MODIFIED,    // preserved  | preserved  | changed
  TRANSFORMED, // preserved  | reassigned | changed
  JUMPED_OUT,  // freed      | saved      | saved
  JUMPED_IN,   // assigned   | restored   | restored
};
END_NAMESPACE_CORE

std::ostream &operator<<(std::ostream &o, simbad::core::EVENT_KIND ek);

#endif
