#ifndef SIMPLE_EVENT_HPP
#define SIMPLE_EVENT_HPP

#include <boost/operators.hpp>

namespace simbad
{
namespace core
{

class simple_event
{
  public:
    explicit simple_event(float t = 0.0f, uint32_t event_type=0,
                          void *particle = nullptr)
        : t(t), event_type(event_type), particle(particle)
    {
    }
    simple_event(simple_event const &) = delete;
    simple_event(simple_event &&) = default;

    simple_event &operator=(simple_event const &) = delete;
    simple_event &operator=(simple_event &&) = default;

    float get_time() const { return t; }
    void set_time(float t) { this->t = t; }

    void set_event_type(uint32_t et) { event_type = et; }
    uint32_t get_event_type() const { return event_type; }

    void set_particle_ptr(void *ptr) { particle = ptr; }
    void *get_particle_ptr() { return particle; }
    void const *get_particle_ptr() const { return particle; }

    template <class P> P *get_particle_ptr_as()
    {
        return static_cast<P *>(get_particle_ptr());
    }
    template <class P> P const *get_particle_ptr_as() const
    {
        return static_cast<P const *>(get_particle_ptr());
    }

  private:
    float t;
    uint32_t event_type;
    void *particle;
};
}
}

#endif // SIMPLE_EVENT_HPP
