#ifndef SPACETIME_HPP
#define SPACETIME_HPP
#include <cstddef>

#include "containers/spacetime/cartesian_spacetime.hpp"
#include "interface/configuration_view.hpp"
#include "parameter_evolution_3d_def.hpp"
#include "particle.hpp"

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
struct spacetime_traits
{
  using particle_type = cell;
  struct particle_ordering
  {
    bool operator()(particle_type const &p1, particle_type const &p2) const
    {
      return p2.event_time() < p1.event_time();
    }
  };
  using coord_scalar = particle_type::position_scalar;
  using coord_vector = particle_type::position_type;
  using dimension_type = std::size_t;
  constexpr static dimension_type dimension = 3;
  constexpr static bool allow_empty_tiles = true;
  struct coord_getter
  {
    coord_vector operator()(particle_type const &p) const
    {
      return p.position();
    }
  };
};
/*
using spacetime_super = simbad::core::cartesian_spacetime<spacetime_traits>;
class spacetime : public simbad::core::cartesian_spacetime<spacetime_traits>,
                  public simbad::core::configuration_view
{
public:
  using spacetime_super::spacetime_super;

  std::size_t size() const override;
  void visit_records(record_visitor visitor) const override;
  simbad::core::attribute_descriptor const &descriptor() const override;
};
*/
using spacetime = simbad::core::cartesian_spacetime<spacetime_traits>;

END_NAMESPACE_PARAMETER_EVOLUTION_3D

#endif // SPACETIME_HPP
