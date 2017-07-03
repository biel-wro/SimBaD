#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP
#include "adhesion_2d_def.hpp"

#include "computational/extractors/generalized_exponential_extractor.hpp"

#include <random>

BEGIN_NAMESPACE_ADHESION_2D
struct model_parameters
{
  model_parameters();
  ~model_parameters();

  static model_parameters
  from_property_tree(simbad::core::property_tree const &mp);
  model_parameters(simbad::core::property_tree const &pt);
  static double range_from_tolerance(double reltol, double ground_pos,
                                     int order);

  double compute_force(double x) const;
  double compute_force_nocutoff(double x) const;
  double compute_potential_nocutoff(double x) const;
  double interaction_range() const;
  double max_jump() const;
  double diffusion() const;
  double brownian_displacement(std::mt19937_64 &rng, double t) const;
  double viscosity() const;
  double viscosus_speed(double dt, double old_speed) const;
  double acceleration_with_friction(double f) const;
  double min_time_step() const;
  double max_time_step() const;

private:
  double m_max_jump;
  double m_min_time_step;
  double m_max_time_step;
  double m_viscosity;
  double m_diffusion;
  double m_max_friction;

  int m_potential_order;
  double m_potential_depth;
  double m_potential_ground_pos;
  double m_potential_cutoff;
};
END_NAMESPACE_ADHESION_2D
#endif // PARAMETERS_HPP
