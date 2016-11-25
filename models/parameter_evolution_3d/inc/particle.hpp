#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "parameter_evolution_3d_fwd.hpp"

#include "accumulator.hpp"
#include "coordinates/coordinates.hpp"
#include "event_kind.hpp"
#include "intrinsic_params.hpp"
#include <random>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
class cell
{
public:
  using coord_scalar = float;
  using coords = simbad::core::coordinates<float, 3>;
  using interaction_accumulator = simbad::core::additive_accumulator<float>;
  using EVENT_KIND = simbad::core::EVENT_KIND;
  cell();
  ~cell();

  coords const &get_coords() const;
  float get_time() const;
  EVENT_KIND get_event_kind() const;

  void include_interaction(cell const &p, model_params const &mp);
  void exclude_interaction(cell const &p, model_params const &mp);

  cell_params &get_params();
  cell_params const &get_params() const;

  void mutate(model_params const &,std::mt19937_64 &rng);

private:
  coords m_coords;
  float m_time;
  cell_params m_intrinsic_params;
  interaction_accumulator m_interaction_acc;
  EVENT_KIND m_event_kind;
};
END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // PARTICLE_HPP
