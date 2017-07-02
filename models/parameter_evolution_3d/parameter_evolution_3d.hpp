#include "parameter_evolution_3d_fwd.hpp"
#include <string>

#include "computational/interactions/generalized_exponential_interaction.hpp"
#include "interface/model.hpp"

#include "model_parameters.hpp"
#include "spacetime.hpp"

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

class parameter_evolution_3d : public simbad::core::model
{
public:

  parameter_evolution_3d(simbad::core::property_tree const &pt);

  simbad::core::attribute_description const &event_descriptor() const override;

  void generate_events(event_visitor, std::size_t nevents) override;
  simbad::core::configuration_view const &
  current_configuration() const override;

  void
  read_configuration(simbad::core::configuration_view const &conf) override;

  double time() const;

  std::size_t particle_count() const;
  void insert(cell c);
  void resample_event(cell &c);
  void pop();

  void check_accumulators();
  simbad::core::attribute particle_attribute(cell const &c,
                                             std::size_t attrname) const;

  spacetime const &current_spacetime() const;

  double compute_birth_rate(cell const &c) const;
  double compute_death_rate(cell const &c) const;
  double compute_success_rate(cell const &c) const;

protected:
  void mutate(cell &c);
  void execute_death(event_visitor v);
  void execute_birth(event_visitor v);

private:
  double m_time;
  std::mt19937_64 m_rng;
  spacetime m_spacetime;
  model_params m_model_params;
  std::unique_ptr<simbad::core::configuration_view> m_configurtation_view;
};

END_NAMESPACE_PARAMETER_EVOLUTION_3D
