#ifndef TRACKING_LISTENER_HPP
#define TRACKING_LISTENER_HPP

#include <memory>

namespace simbad
{
namespace core
{
template <class Callback> class tracking_listener
{
public:
  tracking_listener() : callback() {}
  explicit tracking_listener(Callback c) : callback(std::move(c)) {}
  tracking_listener(tracking_listener &&) = default;
  tracking_listener(tracking_listener const &) = delete;

  tracking_listener &operator=(tracking_listener &&) = default;
  tracking_listener &operator=(tracking_listener const &) = delete;

  template <class T> void notify_constructed(T *ptr) { callback(ptr); }
  template <class T> void notify_copy_constructed(T *ptr) { callback(ptr); }
  template <class T> void notify_move_constructed(T *ptr) { callback(ptr); }
  template <class T> void notify_copy_assigned(T *ptr) { callback(ptr); }
  template <class T> void notify_move_assigned(T *ptr) { callback(ptr); }
  template <class T> void notify_destroyed(T *) { callback(nullptr); }
  template <class T> void notify_forced(T *ptr) { callback(ptr); }
  void set_callback(Callback c) { callback = std::move(c); }
  Callback &get_callback() { return callback; }
  Callback const &get_callback() const { return callback; }
private:
  Callback callback;
};
}
}

#endif // TRACKING_LISTENER_HPP
