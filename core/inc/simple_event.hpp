#ifndef SIMPLE_EVENT_HPP
#define SIMPLE_EVENT_HPP
#include <ostream>

#include "event_kind.hpp"
#include "particle_coords.hpp"
namespace simbad
{
namespace core
{
template <class Time, class Coord, std::size_t DIM>
class simple_event
{
public:
  using time_type = Time;
  using coord_type = Coord;

  static constexpr std::size_t s_dimension() { return DIM; }
  constexpr std::size_t dimension() const { return DIM; }

  explicit simple_event(time_type t = 0.0, EVENT_KIND ek = EVENT_KIND::NONE)
      : t(t), ek(ek)
  {
  }

  void set_time(time_type time) { t = time; }
  time_type &time() { return t; }
  time_type time() const { return t; }

  void set_event_kind(EVENT_KIND event_kind) { ek = event_kind; }
  EVENT_KIND &event_kind() { return ek; }
  EVENT_KIND event_kind() const { return ek; }

  void set_coordinate(size_t cdim, coord_type c) { coords[cdim] = c; }
  coord_type &coordinate_ref(size_t cdim) { return coords[cdim]; }

  coord_type coordinate(size_t cdim) const
  {
    if (cdim >= DIM)
      return coord_type(0);
    return coords[cdim];
  }

private:
  particle_coords<coord_type, DIM> coords;
  time_type t;
  EVENT_KIND ek;
};
}
}
template <class time_type, class coord_type, std::size_t dim>
std::ostream &
operator<<(std::ostream &o,
           simbad::core::simple_event<time_type, coord_type, dim> const &e)
{
  o << "t=" << e.time();

  if (e.dimension() > 0)
    o << " (" << e.coordinate(0);
  for (std::size_t i = 1; i < e.dimension(); ++i)
    o << e.coordinate(i) << ",";

  o << ") " << e.event_kind();

  return o;
}

#endif // SIMPLE_EVENT_HPP
