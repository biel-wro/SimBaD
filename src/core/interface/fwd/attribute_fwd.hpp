#ifndef SIMBAD_CORE_FWD_ATTRIBUTE_FWD_HPP
#define SIMBAD_CORE_FWD_ATTRIBUTE_FWD_HPP

#include <cstddef>
#include <cstdint>
#include "core_def.hpp"
BEGIN_NAMESPACE_CORE
class attribute;
class attribute_description;
class attribute_descriptor;
class attribute_list;
// enums
enum class ATTRIBUTE_KIND : std::uint32_t;
enum class ATTRIBUTE_SCALAR : std::uint32_t;
enum class EVENT_KIND : std::uint32_t;

// template classes
template <class> class array_attribute;

// aliases
using event = attribute_list;

END_NAMESPACE_CORE
#endif
