#ifndef SPACETIME_HPP
#define SPACETIME_HPP
#include <cstddef>

#include "adhesion_2d_def.hpp"
#include "containers/spacetime/cartesian_spacetime.hpp"
#include "particle.hpp"

BEGIN_NAMESPACE_ADHESION_2D
struct spacetime_traits
{
  using particle_type = cell;
  struct particle_ordering
  {
    bool operator()(particle_type const &p1, particle_type const &p2) const
    {
      return p2.next_jump_time() < p1.next_jump_time();
    }
  };
  using coord_scalar = particle_type::coord_scalar;
  using coord_vector = particle_type::position_type;
  using dimension_type = std::size_t;
  constexpr static dimension_type dimension = 2;
  constexpr static bool allow_empty_tiles = true;
  struct coord_getter
  {
    coord_vector operator()(particle_type const &p) const
    {
      return p.position();
    }
  };
};

using spacetime = simbad::core::cartesian_spacetime<spacetime_traits>;

END_NAMESPACE_ADHESION_2D

#endif // SPACETIME_HPP
