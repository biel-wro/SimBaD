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
  parameter_evolution_3d(core::property_tree const &pt);
  ~parameter_evolution_3d();

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

  void check_accumulators() const;
  std::unordered_map<std::size_t, std::size_t> count_clones() const;
  std::unordered_map<std::size_t, std::size_t> count_mutations(
      std::unordered_map<std::size_t, std::size_t> const &clone_counts) const;
  void dump_mutation_stats(std::string const& path) const;
  void dump_mutation_tree(std::string const &path) const;
  void dump_parent_mutations(std::string const &path) const;
  simbad::core::attribute particle_attribute(cell const &c,
                                             std::size_t attrname) const;

  spacetime const &current_spacetime() const;

  double compute_birth_rate(cell const &c) const;
  double compute_death_rate(cell const &c) const;
  double compute_success_rate(cell const &c) const;

protected:
  std::size_t generate_mutation_id();
  void mutate(cell &c);
  void execute_death(event_visitor v);
  void execute_birth(event_visitor v);
  std::vector<std::shared_ptr<cell_params const>> all_mutations() const;

private:
  double m_time;
  std::size_t m_last_muatation_id;
  std::mt19937_64 m_rng;
  spacetime m_spacetime;
  model_params m_model_params;
  std::unique_ptr<core::configuration_view> m_configurtation_view;
  std::vector<std::shared_ptr<cell_params const>> m_all_mutations;
  std::string m_tree_dump_path;
  std::string m_stats_dump_path;
  std::string m_parent_mutations_dump_path;
};

END_NAMESPACE_PARAMETER_EVOLUTION_3D
