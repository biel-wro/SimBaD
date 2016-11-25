#include "parameter_evolution_3d_def.hpp"

#include "model_parameters.hpp"

#include "interface/model.hpp"
#include "spacetime.hpp"

#include <string>

BEGIN_NAMESPACE_PARAMETER_EVOLUTION_3D

class parameter_evolution_3d : public simbad::core::model
{
public:
  static std::string model_name();
  static std::size_t s_dimension();

  std::size_t dimension() const override;
  void generate_events(event_visitor, size_type nevents) override;
  size_type configuration_size() const override;
  void visit_configuration(particle_visitor) const override;

protected:
  double compute_birth_rate(cell_params const &, model_params const &);
  double compute_death_rate(cell_params const &, model_params const &);
  double compute_failure_rate(cell_params const &, model_params const &);

  void mutate_birth_rate(cell_params &, model_params const &);
  void mutate_death_rate(cell_params &, model_params const &);
  void mutate_failure_rate(cell_params &, model_params const &);

  void resample_event(cell &p);
  void generate_event(event_visitor v);
  void execute_death(event_visitor v);
  void execute_birth(event_visitor v);

private:
  float m_interaction_range;
  spacetime m_spacetime;
};

END_NAMESPACE_PARAMETER_EVOLUTION_3D
