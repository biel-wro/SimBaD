#ifndef TRACKEE_H
#define TRACKEE_H

#include <algorithm>
#include <type_traits>

namespace simbad
{
namespace core
{

template <class Listener, class Derived>
class trackee
{
  public:
    trackee() : listener() { listener.notify_constructed(raw_ptr()); }
    explicit trackee(Listener c) : listener(std::move(c))
    {
        c.notify_constructed(raw_ptr());
    }

    trackee(const trackee &o) : listener(o.listener)
    {
        listener.notify_copy_constructed(raw_ptr());
    }
    trackee(trackee &&o) : listener(std::move(o.listener))
    {
        o.listener = Listener();
        listener.notify_move_constructed(raw_ptr());
    }

    trackee &operator=(trackee const &o)
    {
        if (&o != this)
        {
            listener = o.listener;
            listener.notify_copy_assigned(raw_ptr());
        }
        return *this;
    }

    trackee &operator=(trackee &&o)
    {
        if (&o != this)
        {
            std::swap(listener, o.listener);
            listener.notify_move_assigned(raw_ptr());
        }
        return *this;
    }

    ~trackee() { listener.notify_destroyed(raw_ptr()); }

    void force_listener_update() { listener.nofity_forced(raw_ptr()); }

    void set_listener(Listener _new) { listener = std::move(_new); }
    Listener &get_listener() { return listener; }
    Listener const &get_listener() const { return listener; }

    Derived const *raw_ptr() const { return static_cast<Derived *>(this); }
    Derived *raw_ptr() { return static_cast<Derived *>(this); }

  protected:
    Listener listener;
};
}
}
#endif // TRACKEE_H
