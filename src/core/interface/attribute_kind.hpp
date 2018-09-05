#ifndef ATTRIBUTE_KIND_HPP
#define ATTRIBUTE_KIND_HPP

#include "core_def.hpp"

#include <cstdint>
#include <iosfwd>

BEGIN_NAMESPACE_CORE
enum class ATTRIBUTE_KIND : std::uint32_t
{
  /* Special attributes */
  PARTICLE_UID,      // optional, unique, integer, scalar
  PARTICLE_POSITION, // optional, numeric values, fixed dimension
  EVENT_UID,         // optional, unique, integer, scalar
  EVENT_TIME,        // numeric scalar
  EVENT_DELTA_TIME,  // integer scalar
  EVENT_KIND,        // special case, same values as in event_kind enum
  /* Model-dependent attributes */
  INTRINSIC,   // attributes stored in particle, non-recomputable
  ACCUMULATED, // attributes stored in particle, recomputable
  OBSERVABLE,  // attributes not-stored in particle, with physical meaning
  INFO,        // other attributes
  UNKNOWN,     // no information provided
};

std::ostream &operator<<(std::ostream &, ATTRIBUTE_KIND);
std::istream &operator>>(std::istream &, ATTRIBUTE_KIND &);

END_NAMESPACE_CORE
#endif // ATTRIBUTE_KIND_HPP
