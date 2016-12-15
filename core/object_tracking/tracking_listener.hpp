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
  //  tracking_listener() : callback() {}
  explicit tracking_listener(Callback c = Callback()) : m_callback(std::move(c))
  {
  }
  tracking_listener(tracking_listener &&o) : m_callback(std::move(o.m_callback))
  {
    o.m_callback = Callback();
  }

  tracking_listener(tracking_listener const &) = delete;

  tracking_listener &operator=(tracking_listener &&o)
  {
    if(this == std::addressof(o))
      return *this;
    m_callback = std::move(o.m_callback);
    o.m_callback = Callback();
    return *this;
  }
  tracking_listener &operator=(tracking_listener const &) = delete;
  template <class T> void notify_constructed(T *ptr) { m_callback(ptr); }
  template <class T> void notify_copy_constructed(T *ptr) { m_callback(ptr); }
  template <class T> void notify_move_constructed(T *ptr) { m_callback(ptr); }
  template <class T> void notify_copy_assigned(T *ptr) { m_callback(ptr); }
  template <class T> void notify_move_assigned(T *ptr) { m_callback(ptr); }
  template <class T> void notify_destroyed(T *) { m_callback(nullptr); }
  template <class T> void notify_forced(T *ptr) { m_callback(ptr); }
  void set_callback(Callback c) { m_callback = std::move(c); }
  Callback &get_callback() { return m_callback; }
  Callback const &get_callback() const { return m_callback; }
private:
  Callback m_callback;
};
}
}

#endif // TRACKING_LISTENER_HPP
