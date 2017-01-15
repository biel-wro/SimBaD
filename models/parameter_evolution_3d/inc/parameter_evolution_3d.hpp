#include "parameter_evolution_3d_def.hpp"

#include "model_parameters.hpp"

#include "computational/interactions/generalized_exponential_interaction.hpp"
#include "interface/model.hpp"
#include "spacetime.hpp"

#include <string>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

class parameter_evolution_3d : public simbad::core::model
{
public:
  parameter_evolution_3d(simbad::core::property_tree const &pt);

  void generate_events(event_visitor, size_type nevents) override;
  size_type configuration_size() const override;
  void visit_configuration(particle_visitor) const override;
  simbad::core::attribute_descriptor const &new_attr_map() const override;
  void read_configuration(configuration_view const &conf) override;

  double time() const;

  void insert(cell c);
  void resample_event(cell &c);
  void pop();

  void check_accumulators();
  simbad::core::attribute new_attribute(cell const &c, std::size_t idx) const;
  simbad::core::attribute attribute(cell const &c, std::size_t attrname) const;

protected:
  double compute_birth_rate(cell const &c) const;
  double compute_death_rate(cell const &c) const;
  double compute_success_rate(cell const &c) const;

  void mutate(cell &c);
  void execute_death(event_visitor v);
  void execute_birth(event_visitor v);

private:
  double m_time;
  std::mt19937_64 m_rng;
  spacetime m_spacetime;
  model_params m_model_params;
};

END_NAMESPACE_PARAMETER_EVOLUTION_3D
