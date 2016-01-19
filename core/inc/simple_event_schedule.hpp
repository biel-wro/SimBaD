#ifndef SIMPLE_EVENT_SCHEDULE_HPP
#define SIMPLE_EVENT_SCHEDULE_HPP

#include <cstdint>

#include <boost/operators.hpp>

#include "event_kind.hpp"
namespace simbad
{
namespace core
{

class simple_event_schedule
    : boost::partially_ordered<
          simple_event_schedule,
          boost::partially_ordered2<simple_event_schedule, float>>
{
  public:
    simple_event_schedule(void *particle)
        : t(0), event_kind(EVENT_KIND::NONE), particle(particle)
    {
    }
    explicit simple_event_schedule(float t = 0.0f,
                                   EVENT_KIND ek = EVENT_KIND::NONE,
                                   void *particle = nullptr)
        : t(t), event_kind(ek), particle(particle)
    {
    }
    simple_event_schedule(simple_event_schedule const &) = delete;
    simple_event_schedule(simple_event_schedule &&) = default;

    simple_event_schedule &operator=(simple_event_schedule const &) = delete;
    simple_event_schedule &operator=(simple_event_schedule &&) = default;

    float get_time() const { return t; }
    void set_time(float t) { this->t = t; }
    void increase_time(double time_offset) { this->t += time_offset; }

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

    bool operator==(simple_event_schedule const &other) const
    {
        return t == other.t;
    }
    bool operator<(simple_event_schedule const &other) const
    {
        return t < other.t;
    }
    bool operator==(float t2) const { return t < t2; }
    bool operator<(float t2) const { return t < t2; }

  private:
    float t;
    EVENT_KIND event_kind;
    void *particle;
};
}
}

#endif // SIMPLE_EVENT_HPP
