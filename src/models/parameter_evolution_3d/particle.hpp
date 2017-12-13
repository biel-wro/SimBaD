#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "parameter_evolution_3d_fwd.hpp"

#include "interface/interface_fwd.hpp"

#include "computational/templates/accumulator.hpp"
#include "coordinates/coordinates.hpp"
#include "interface/event_kind.hpp"

#include <random>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D
class cell
{
public:
  using position_scalar = float;
  using position_type = simbad::core::coordinates<float, 3>;
  using interaction_accumulator = simbad::core::additive_accumulator<float>;
  using EVENT_KIND = simbad::core::EVENT_KIND;
  static core::attribute_description const &description();

  cell(position_type pos, std::shared_ptr<cell_params> params_ptr);
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

  cell_params const &params() const;
  std::shared_ptr<cell_params const> params_ptr() const;
  void set_params_ptr(std::shared_ptr<cell_params const> params_ptr);

  double density() const;

private:
  position_type m_position;
  float m_time;
  std::shared_ptr<cell_params const> m_params_ptr;
  double m_interaction_acc;
  EVENT_KIND m_event_kind;
};
END_NAMESPACE_PARAMETER_EVOLUTION_3D
#endif // PARTICLE_HPP
