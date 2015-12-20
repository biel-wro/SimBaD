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
    explicit simple_event(float t = 0.0f, void *particle = nullptr)
        : t(t), particle(particle)
    {
    }
    float get_time() const { return t; }
    void set_time(float t) { this->t = t; }

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
    void *particle;
};
}
}

#endif // SIMPLE_EVENT_HPP
