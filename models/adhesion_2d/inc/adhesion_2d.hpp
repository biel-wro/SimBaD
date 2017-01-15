#include "adhesion_2d_def.hpp"

#include "interface/configuration_reader.hpp"
#include "interface/model.hpp"
#include "interface/particle.hpp"
#include "model_parameters.hpp"
#include "spacetime.hpp"

#include <random>
#include <string>

BEGIN_NAMESPACE_ADHESION_2D

std::unique_ptr<simbad::core::model>
from_property_tree(simbad::core::property_tree const &);

class adhesion_2d : public simbad::core::model
{
public:
  explicit adhesion_2d(simbad::core::property_tree const &pt);
  explicit adhesion_2d(model_parameters mp, double tile_size = 1.0,
                       std::uint64_t seed = 0);
  void generate_events(event_visitor, size_type nevents) override;

  size_type configuration_size() const override;
  void visit_configuration(particle_visitor) const override;
  simbad::core::attribute_descriptor const &new_attr_map() const override;
  void read_configuration(configuration_view const &configuration) override;

  double time() const;
  void print_nicely(std::string header);

protected:
  using time_type = double;
  using position_type = cell::position_type;
  using velocity_type = cell::velocity_type;
  using force_type = cell::force_type;
  using pressure_type = cell::pressure_type;

  double optimal_time_step(const cell &p) const;
  void update_time(cell &p, bool new_timestep) const;
  void update_velocity(cell &p, double dt) const;

  velocity_type viscosus_velocity(double dt, velocity_type v) const;
  std::pair<force_type, pressure_type>
  compute_force(position_type const &target, position_type const &other) const;

  void resample_all();

private:
  time_type m_time;
  model_parameters m_parameters;
  spacetime m_spacetime;
  std::mt19937_64 rng;
};

END_NAMESPACE_ADHESION_2D
