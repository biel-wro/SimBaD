#ifndef TRACKEE_H
#define TRACKEE_H

#include <algorithm>
#include <type_traits>

namespace simbad
{
namespace core
{
template <class Listener, class Derived> class trackee
{
public:
  explicit trackee(Listener c = Listener()) : m_listener(std::move(c))
  {
    m_listener.notify_constructed(raw_ptr());
  }

  trackee(const trackee &o) : m_listener(o.m_listener)
  {
    m_listener.notify_copy_constructed(raw_ptr());
  }
  trackee(trackee &&o) : m_listener(std::move(o.m_listener))
  {
    o.m_listener = Listener();
    m_listener.notify_move_constructed(raw_ptr());
  }

  trackee &operator=(trackee const &o)
  {
    if(&o != this)
    {
      m_listener = o.m_listener;
      m_listener.notify_copy_assigned(raw_ptr());
    }
    return *this;
  }

  trackee &operator=(trackee &&o)
  {
    if(&o != this)
    {
      std::swap(m_listener, o.m_listener);
      m_listener.notify_move_assigned(raw_ptr());
    }
    return *this;
  }

  ~trackee() { m_listener.notify_destroyed(raw_ptr()); }

  void force_listener_update() { m_listener.notify_forced(raw_ptr()); }

  void set_listener(Listener _new) { m_listener = std::move(_new); }
  Listener &get_listener() { return m_listener; }
  Listener const &get_listener() const { return m_listener; }

  Derived const *raw_ptr() const { return static_cast<Derived *>(this); }
  Derived *raw_ptr() { return static_cast<Derived *>(this); }

protected:
  Listener m_listener;
};
}
}
#endif // TRACKEE_H
