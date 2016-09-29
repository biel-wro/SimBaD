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
      : m_time(0), m_event_kind(EVENT_KIND::NONE), m_particle_ptr(particle)
  {
  }

  explicit simple_event_schedule(float t = 0.0f,
                                 EVENT_KIND ek = EVENT_KIND::NONE,
                                 void *particle = nullptr)
      : m_time(t), m_event_kind(ek), m_particle_ptr(particle)
  {
  }
  simple_event_schedule(simple_event_schedule const &) = delete;
  simple_event_schedule(simple_event_schedule &&) = default;

  simple_event_schedule &operator=(simple_event_schedule const &) = delete;
  simple_event_schedule &operator=(simple_event_schedule &&) = default;

  float get_time() const { return m_time; }
  void set_time(float t) { this->m_time = t; }
  void increase_time(double time_offset) { this->m_time += time_offset; }
  void set_event_kind(EVENT_KIND ek) { m_event_kind = ek; }
  EVENT_KIND get_event_kind() const { return m_event_kind; }
  void set_particle_ptr(void *ptr) { m_particle_ptr = ptr; }
  void *get_particle_ptr() { return m_particle_ptr; }
  void const *get_particle_ptr() const { return m_particle_ptr; }
  template <class P> P *get_particle_ptr_as()
  {
    return static_cast<P *>(get_particle_ptr());
  }
  template <class P> P const *get_particle_ptr_as() const
  {
    return static_cast<P const *>(get_particle_ptr());
  }

  template <class P> P *get_particle_ptr_as_nonconst() const
  {
    return static_cast<P *>(m_particle_ptr);
  }

  bool operator==(simple_event_schedule const &other) const
  {
    return m_time == other.m_time;
  }
  bool operator<(simple_event_schedule const &other) const
  {
    return m_time < other.m_time;
  }
  bool operator==(float t2) const { return m_time < t2; }
  bool operator<(float t2) const { return m_time < t2; }
private:
  float m_time;
  EVENT_KIND m_event_kind;
  void *m_particle_ptr;
};
}
}

#endif // SIMPLE_EVENT_HPP
