#ifndef SPACETIME_HPP
#define SPACETIME_HPP
#include <cstddef>


#include "parameter_evolution_3d_def.hpp"
#include "spacetime/cartesian_spacetime.hpp"
#include "particle.hpp"

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
struct spacetime_traits
{
  using particle = cell;
  struct ordering
  {
    bool operator()(particle const &p1, particle const &p2) const
    {
      return p2.get_time() < p1.get_time();
    }
  };
  using coord_scalar = particle::coord_scalar;
  using coord_vector = particle::coords;
  using dimension_type = std::size_t;
  constexpr static dimension_type dimension = 3;
  struct coord_getter
  {
    coord_vector operator()(particle const &p) const { return p.get_coords(); }
  };
};

using spacetime = simbad::core::cartesian_spacetime<spacetime_traits>;

END_NAMESPACE_PARAMETER_EVOLUTION_3D

#endif // SPACETIME_HPP
