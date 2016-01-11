#ifndef EVENT_KIND_HPP
#define EVENT_KIND_HPP

#include <ios>
#include <type_traits>

#include "event_kind.hpp"
namespace simbad
{
namespace core
{
enum class EVENT_KIND : uint32_t
{
    NONE,
    BIRTH,
    DEATH,
    JUMP
};
}
}

std::ostream &operator<<(std::ostream &o, simbad::core::EVENT_KIND ek);

#endif // EVENT_KIND_HPP
