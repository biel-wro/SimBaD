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
// static std::string model_name();
// static std::size_t dimension();

class adhesion_2d : public simbad::core::model
{
public:
  explicit adhesion_2d(simbad::core::property_tree const &pt);
  explicit adhesion_2d(model_parameters mp, double tile_size = 1.0,
                       std::uint64_t seed = 0);
  void generate_events(event_visitor, size_type nevents) override;
  size_type dimension() const override;
  size_type configuration_size() const override;
  void visit_configuration(particle_visitor) const override;

  void read_configuration(configuration_view const &configuration) override;

  double time() const;

protected:
  using position_type = particle::position_type;
  using velocity_type = particle::velocity_type;
  using acceleration_type = particle::acceleration_type;

  acceleration_type compute_acceleration(particle const &p1,
                                         particle const &p2) const;
  velocity_type viscosus_velocity(double dt, velocity_type v) const;

  void update_time(particle &p) const;
  void include(particle &p, acceleration_type const &acc) const;
  void exclude(particle &p, acceleration_type const &acc) const;
  void include_all_twosided(particle &p);
  void exclude_all_onesided(const particle &particle_tmp);
  void resample_all();

private:
  void print_nicely(std::string header);

  double m_time;
  model_parameters m_parameters;
  spacetime m_spacetime;
  std::mt19937_64 rng;
};

END_NAMESPACE_ADHESION_2D
