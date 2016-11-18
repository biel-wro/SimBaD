#ifndef SPACETIME_HPP
#define SPACETIME_HPP
#include <cstddef>


#include "adhesion_2d_def.hpp"
#include "spacetime/cartesian_spacetime.hpp"
#include "particle.hpp"

BEGIN_NAMESPACE_ADHESION_2D
struct spacetime_traits
{
  using particle = cell;
  struct ordering
  {
    bool operator()(particle const &p1, particle const &p2) const
    {
      return p2.event_time() < p1.event_time();
    }
  };
  using coord_scalar = particle::coord_scalar;
  using coord_vector = particle::position_type;
  using dimension_type = std::size_t;
  constexpr static dimension_type dimension = 3;
  struct coord_getter
  {
    coord_vector operator()(particle const &p) const { return p.position(); }
  };
};

using spacetime = simbad::core::cartesian_spacetime<spacetime_traits>;

END_NAMESPACE_ADHESION_2D

#endif // SPACETIME_HPP
