#ifndef UNI_TRACKEE_HPP
#define UNI_TRACKEE_HPP

#include "trackee.hpp"
#include "tracking_listener.hpp"

namespace simbad
{
namespace core
{
template <class Callback, class Derived>
class uni_trackee : public trackee<tracking_listener<Callback>, Derived>
{
public:
  using callback_type = Callback;
  using my_listener = tracking_listener<callback_type>;
  using my_base = trackee<my_listener, Derived>;
  // uni_trackee() = default;
  explicit uni_trackee(Callback c = Callback())
      : my_base(my_listener(std::move(c)))
  {
  }

  uni_trackee(uni_trackee &&) = default;
  uni_trackee(uni_trackee const &) = delete;

  uni_trackee &operator=(uni_trackee &&) = default;
  uni_trackee &operator=(uni_trackee const &) = delete;

  void set_callback(callback_type callback)
  {
    my_base::set_listener(my_listener(std::move(callback)));
  }
  callback_type &get_callback()
  {
    my_listener &listener = my_base::get_listener();
    return listener.get_callback();
  }
  callback_type const &get_callback() const
  {
    my_listener const &listener = my_base::get_listener();
    return listener.get_callback();
  }
};
}
}

#endif // UNI_TRACKEE_HPP
