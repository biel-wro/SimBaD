#ifndef TRACKEE_BASE_H
#define TRACKEE_BASE_H

#include <type_traits>

template <class Callback> class trackee
{
  public:


    trackee() : c() { c.notify_constructed(raw_ptr()); }
    explicit trackee(Callback c) : c(std::move(c))
    {
        c.notify_constructed(raw_ptr());
    }

    trackee(const trackee &o) : c(o.c) { c.notify_copy_constructed(raw_ptr()); }
    trackee(trackee &&o) : c(std::move(o.c))
    {
        o.c = Callback();
        c.notify_move_constructed(raw_ptr());
    }

    trackee &operator=(trackee const &o)
    {
        if (&o != this)
        {
            c = o.c;
            c.notify_copy_assigned(raw_ptr());
        }
        return *this;
    }

    trackee &operator=(trackee &&o)
    {
        if (&o != this)
        {
            std::swap(c,o.c);
            c.notify_move_assigned(raw_ptr());
        }
        return *this;
    }

    ~trackee() { c.notify_destroyed(raw_ptr()); }

    void set_callback(Callback _new) { c = std::move(_new); }
    Callback get_callback() { return c; }

    void const *raw_ptr() const { return reinterpret_cast<const void *>(this); }
    void *raw_ptr() { return reinterpret_cast<void *>(this); }

  protected:
    Callback c;
};

#endif // TRACKEE_BASE_H
