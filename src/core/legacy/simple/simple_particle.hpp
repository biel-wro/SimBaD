#ifndef SIMPLE_PARTICLE_HPP
#define SIMPLE_PARTICLE_HPP

#include "interface/interface_fwd.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <initializer_list>

#include <boost/operators.hpp>

BEGIN_NAMESPACE_CORE

template <size_t DIM, class coord_type> class simple_particle

{
public:
  using cref = const simple_particle &;
  using coord_array_type = std::array<coord_type, DIM>;

  explicit simple_particle(std::initializer_list<coord_type> il)
  {
    std::copy(il.begin(), il.end(), coords.begin());
  }
  explicit simple_particle(coord_array_type c) : coords(std::move(c)) {}
  simple_particle(simple_particle const &) = default;
  simple_particle(simple_particle &&o) = default;
  simple_particle &operator=(simple_particle const &) = default;
  simple_particle &operator=(simple_particle &&o) = default;

  std::size_t s_dimension() { return DIM; }

  std::size_t dimension() const { return s_dimension(); }

  coord_type coordinate(std::size_t cdim) const { return coords[cdim]; }
  template <size_t cdim> void set_coordinate(coord_type c) { coords[cdim] = c; }
  template <size_t cdim> coord_type get_coordinate() const
  {
    if(cdim >= DIM)
      return coord_type(0);

    return coords[cdim];
  }

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
