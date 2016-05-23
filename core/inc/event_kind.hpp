#ifndef EVENT_KIND_HPP
#define EVENT_KIND_HPP

#include "core_fwd.hpp"

#include <iosfwd>


BEGIN_NAMESPACE_CORE


enum class EVENT_KIND : uint32_t
{
    NONE,
    BIRTH,
    DEATH,
//    JUMP
};

END_NAMESPACE_CORE



std::ostream &operator<<(std::ostream &o, simbad::core::EVENT_KIND ek);

#endif // EVENT_KIND_HPP
