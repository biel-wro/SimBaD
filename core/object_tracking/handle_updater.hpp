#ifndef HANDLE_UPDATER_HPP
#define HANDLE_UPDATER_HPP

#include <utility>

namespace simbad
{
namespace core
{
template <class handle_type> struct ptr_updater_by_event_handle_base
{
  ptr_updater_by_event_handle_base() : handle(nullptr) {}
  explicit ptr_updater_by_event_handle_base(handle_type h) : handle(h) {}

  template <class P> void operator()(P *ptr)
  {
    if(handle != handle_type(nullptr))
    {
      (*handle).set_particle_ptr(ptr);
    }
  }
  void operator()(std::nullptr_t)
  {
    operator()(static_cast<handle_type *>(nullptr));
  }

  void set_handle(handle_type h) { handle = std::move(h); }
  handle_type get_handle() const { return handle; }

private:
  handle_type handle;
};
}
}

#endif // HANDLE_UPDATER_HPP
