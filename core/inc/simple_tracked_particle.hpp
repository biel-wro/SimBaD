#ifndef SIMPLE_TRACKED_PARTICLE_HPP
#define SIMPLE_TRACKED_PARTICLE_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <initializer_list>

#include <boost/operators.hpp>

#include "handle_trackee.hpp"
#include "ptr_updater.hpp"
#include "simple_event_queue.hpp"
#include "trackee.hpp"

BEGIN_NAMESPACE_CORE
// TODO should be derived from simple_particle
template <size_t DIM, class coord_type>
class simple_tracked_particle
    : public handle_trackee<simbad::core::simple_event_queue::handle_type,
                            simple_tracked_particle<DIM, coord_type>>

{
public:
  using cref = const simple_tracked_particle<DIM, coord_type> &;
  using handle_type = simbad::core::simple_event_queue::handle_type;
  using coord_array_type = std::array<coord_type, DIM>;
  using my_base =
      handle_trackee<handle_type, simple_tracked_particle<DIM, coord_type>>;

  explicit simple_tracked_particle(handle_type h = handle_type(nullptr))
      : my_base(std::move(h))
  {
    std::fill(coords.begin(), coords.end(), coord_type(0));
  }

  explicit simple_tracked_particle(std::initializer_list<coord_type> il,
                                   handle_type h = handle_type(nullptr))
      : my_base(std::move(h))
  {
    std::copy(il.begin(), il.end(), coords.begin());
  }
  explicit simple_tracked_particle(coord_array_type c,
                                   handle_type h = handle_type(nullptr))
      : my_base(std::move(h)), coords(std::move(c))
  {
  }
  simple_tracked_particle(simple_tracked_particle const &) = delete;
  simple_tracked_particle(simple_tracked_particle &&o) = default;
  simple_tracked_particle &operator=(simple_tracked_particle const &) = delete;
  simple_tracked_particle &operator=(simple_tracked_particle &&o) = default;

  std::size_t s_dimension() { return DIM; }
  std::size_t dimension() const { return DIM; }

  coord_type coordinate(std::size_t cdim) const { return coords[cdim]; }
  template <size_t cdim>
  void set_coordinate(coord_type c)
  {
    coords[cdim] = c;
  }
  template <size_t cdim>
  coord_type get_coordinate() const
  {
    if (cdim >= DIM)
      return coord_type(0);

    return coords[cdim];
  }

  void set_handle(handle_type handle) { my_base::set_handle(handle); }
  handle_type get_handle() const { return my_base::get_handle(); }

protected:
  friend bool operator==(cref p1, cref p2) { return p1.coords == p2.coords; }
  friend bool operator!=(cref p1, cref p2) { return !(p1 == p2); }
  friend bool operator<(cref p1, cref p2) { return p1.coords < p2.coords; }
  friend bool operator>(cref p1, cref p2) { return p2 < p1; }
  friend bool operator<=(cref p1, cref p2) { return p1 < p2 || p1 == p2; }
  friend bool operator>=(cref p1, cref p2) { return p1 > p2 || p1 == p2; }

private:
  coord_array_type coords;
};

END_NAMESPACE_CORE
#endif
