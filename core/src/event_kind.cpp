#include "event_kind.hpp"

#include <iostream>
#include <stdexcept>
#include <type_traits>

std::ostream &operator<<(std::ostream &o, simbad::core::EVENT_KIND ek)
{
    using simbad::core::EVENT_KIND;
    std::string str;
    switch (ek)
    {
    case EVENT_KIND::NONE:
        str = "NONE";
        break;
    case EVENT_KIND::BIRTH:
        str = "BIRTH";
        break;
    case EVENT_KIND::DEATH:
        str = "DEATH";
        break;
//    case EVENT_KIND::JUMP:
//        str = "JUMP";
//        break;
    default:
        using num_type =
            typename std::underlying_type<simbad::core::EVENT_KIND>::type;
        num_type val = static_cast<num_type>(ek);
        throw std::runtime_error("bad enum of value " + std::to_string(val));
    }

    return o << str;
}
