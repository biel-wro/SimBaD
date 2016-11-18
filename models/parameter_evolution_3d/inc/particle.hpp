#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "parameter_evolution_3d_fwd.hpp"

#include "accumulator.hpp"
#include "coordinates/coordinates.hpp"
#include "event_kind.hpp"

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
class particle
{
public:
  using coord_scalar = float;
  using coords = simbad::core::coordinates<float, 3>;
  using interaction_accumulator = simbad::core::additive_accumulator<float>;
  using EVENT_KIND = simbad::core::EVENT_KIND;
  struct intrinsic_params
  {
    float m_lifespan;
    float m_mutation_prob;
    float m_birth_coef;
    float m_interaction_coef;
  };
  particle();
  ~particle();

  coords const &get_coords() const;
  float get_time() const;
  EVENT_KIND get_event_kind() const;

  void include_interaction(particle const &p, model_parameters const &mp);
  void exclude_interaction(particle const &p, model_parameters const &mp);

  intrinsic_params &get_params();
  intrinsic_params const &get_params() const;

private:
  coords m_coords;
  float m_time;
  intrinsic_params m_intrinsic_params;
  interaction_accumulator m_interaction_acc;
  EVENT_KIND m_event_kind;
};
END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // PARTICLE_HPP
