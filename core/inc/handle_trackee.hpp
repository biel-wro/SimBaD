#ifndef HANDLE_TRACKEE_HPP
#define HANDLE_TRACKEE_HPP

#include "handle_updater.hpp"
#include "uni_trackee.hpp"

#include <memory>

namespace simbad
{
namespace core
{
template <class Handle, class Derived>
struct handle_trackee
    : public uni_trackee<ptr_updater_by_event_handle_base<Handle>, Derived>
{
    using handle_type = Handle;
    using my_updater = ptr_updater_by_event_handle_base<handle_type>;
    using my_base = uni_trackee<my_updater, Derived>;

    explicit handle_trackee(handle_type handle = handle_type())
        : my_base(my_updater(std::move(handle)))
    {
    }

    void set_handle(handle_type h) { my_base::set_callback(my_updater(h)); }
    handle_type get_handle() const
    {
        return my_base::get_callback().get_handle();
    }
};
}
}

#endif // HANDLE_TRACKEE_HPP
