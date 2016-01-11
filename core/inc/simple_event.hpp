#ifndef SIMPLE_EVENT_HPP
#define SIMPLE_EVENT_HPP

#include <cstdint>

#include <boost/operators.hpp>

#include "event_kind.hpp"
namespace simbad
{
namespace core
{

class simple_event
    : boost::partially_ordered<simple_event,
                               boost::partially_ordered2<simple_event, float>>
{
  public:
    explicit simple_event(float t = 0.0f, EVENT_KIND ek = EVENT_KIND::NONE,
                          void *particle = nullptr)
        : t(t), event_kind(ek), particle(particle)
    {
    }
    simple_event(simple_event const &) = delete;
    simple_event(simple_event &&) = default;

    simple_event &operator=(simple_event const &) = delete;
    simple_event &operator=(simple_event &&) = default;

    float get_time() const { return t; }
    void set_time(float t) { this->t = t; }
    void increase_time(double time_offset) { t += time_offset; }

    void set_event_kind(EVENT_KIND ek) { event_kind = ek; }
    EVENT_KIND get_event_kind() const { return event_kind; }

    void set_particle_ptr(void *ptr) { particle = ptr; }
    void *get_particle_ptr() { return particle; }
    void const *get_particle_ptr() const { return particle; }

    template <class P>
    P *get_particle_ptr_as()
    {
        return static_cast<P *>(get_particle_ptr());
    }
    template <class P>
    P const *get_particle_ptr_as() const
    {
        return static_cast<P const *>(get_particle_ptr());
    }

    template <class P>
    P *get_particle_ptr_as_nonconst() const
    {
        return static_cast<P *>(particle);
    }

    bool operator==(simple_event const &other) const{
      return t==other.t;
    }
    bool operator<(simple_event const &other) const{
      return t<other.t;
    }
    bool operator==(float t2) const{
      return t < t2;
    }
    bool operator<(float t2) const {
      return t < t2;
    }


  private:
    float t;
    EVENT_KIND event_kind;
    void *particle;
};
}
}

#endif // SIMPLE_EVENT_HPP
