#ifndef PTR_UPDATER_HPP
#define PTR_UPDATER_HPP

#include "tracking_listener.hpp"

#include <cstddef>

namespace simbad
{
namespace core
{

template <class Pointee> struct ptr_updater
{
    ptr_updater() : listener(nullptr) {}
    explicit ptr_updater(Pointee *&ref_listener) : listener(&ref_listener) {}

    void operator()(Pointee *ptr)
    {
        if (nullptr != listener)
            *listener = ptr;
    }

    void operator()(std::nullptr_t)
    {
        operator()(static_cast<Pointee *>(nullptr));
    }
    Pointee **listener;
};
using void_ptr_updater = ptr_updater<void>;
}
}
#endif // PTR_UPDATER_HPP
