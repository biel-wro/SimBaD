#ifndef SIMPLE_EXP_2D_HPP
#define SIMPLE_EXP_2D_HPP

#include <boost/property_tree/ptree_fwd.hpp>

#include "simple_exp_2d_def.hpp"

#include "model.hpp"
#include "simple_event_queue.hpp"
#include "space.hpp"

#include "config.hpp"
#include "dispersion.hpp"
#include "kernel.hpp"
#include "particle_2d.hpp"
#include "saturation_intensity.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D
class simple_exp_2d : public simbad::core::model
{
public:
  using particle_type = particle;
  using queue_type = simbad::core::simple_event_queue;
  using space_type = simbad::core::space<particle_type, config>;
  explicit simple_exp_2d(dispersion disp, kernel interaction_kernel,
                         saturation_intensity s, std::mt19937_64 random_engine,
                         std::exponential_distribution<> death_distribution,
                         double tile_size = 1);

  ~simple_exp_2d() override;

  void generate_events(event_visitor, size_t nevents) override;
  std::size_t configuration_size() const override;
  void visit_configuration(particle_visitor) const override;

  void set_configuration();
  double get_time() const;

protected:
  void update_time(double new_time);
  void reset_queue();
  void full_update_particle(particle_type &p);
  void lazy_update_particle_exclude(particle_type &p, particle_type const &o);
  void lazy_update_particle_include(particle_type &p, particle_type const &o);

  void remove_particle(particle_type &p);
  particle_type &add_particle(config::space_coords coords);
  void resample_event(particle_type &p);
  void realize_event(particle_type &p);

private:
  double m_time;
  std::exponential_distribution<> m_death_distribution;
  queue_type m_queue;
  space_type m_space;
  dispersion m_dispersion;
  kernel m_interaction_kernel;
  saturation_intensity m_saturation_intensity;
  std::mt19937_64 m_random_engine;
};
END_NAMESPACE_SIMPLE_EXP_2D
#endif // SIMPLE_EXP_2D_HPP
