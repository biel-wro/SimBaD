#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "parameter_evolution_3d_fwd.hpp"

#include "core_fwd.hpp"

#include "computational/templates/accumulator.hpp"
#include "coordinates/coordinates.hpp"
#include "interface/event_kind.hpp"

#include "intrinsic_params.hpp"

#include <random>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
class cell
{
public:
  using position_scalar = float;
  using position_type = simbad::core::coordinates<float, 3>;
  using interaction_accumulator = simbad::core::additive_accumulator<float>;
  using EVENT_KIND = simbad::core::EVENT_KIND;
  cell(position_type pos, cell_params params);
  ~cell();

  position_type const &position() const;
  void set_position(position_type const &pos);
  float event_time() const;
  void set_event_time(float t);
  EVENT_KIND event_kind() const;
  void set_event_kind(EVENT_KIND ek);

  void reset_interaction();
  void include_interaction(cell const &p, model_params const &mp);
  void exclude_interaction(cell const &p, model_params const &mp);

  cell_params &params();
  cell_params const &params() const;

  double density() const;
  void mutate(model_params const &, std::mt19937_64 &rng);

private:
  position_type m_position;
  float m_time;
  cell_params m_params;
  double m_interaction_acc;
  EVENT_KIND m_event_kind;
};
END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // PARTICLE_HPP
