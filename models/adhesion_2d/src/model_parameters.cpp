#include "model_parameters.hpp"
#include "property_tree.hpp"

#include <cmath>

BEGIN_NAMESPACE_ADHESION_2D
model_parameters::model_parameters() {}
model_parameters::~model_parameters() {}
model_parameters
model_parameters::from_property_tree(const simbad::core::property_tree &ptree)
{
  model_parameters mp;
  mp.m_max_jump = ptree.get<double>("max_jump");
  mp.m_viscosity = ptree.get<double>("viscosity");
  mp.m_diffusion = ptree.get<double>("diffusion");

  mp.m_potential_depth = ptree.get<double>("potential.depth");
  mp.m_potential_ground_pos =
      ptree.get<double>("potential.groundstate_location");
  mp.m_potential_order = ptree.get<int>("potential.order", 6);

  double reltol = ptree.get<double>("potential.tolerance");
  mp.m_potential_cutoff = range_from_tolerance(
      reltol, mp.m_potential_ground_pos, mp.m_potential_order);

  return mp;
}

model_parameters::model_parameters(const simbad::core::property_tree &ptree)
    : m_max_jump(ptree.get<double>("max_jump")),
      m_min_time_step(ptree.get<double>("time_steps.min", 0)),
      m_max_time_step(ptree.get<double>(
          "time_steps.max", std::numeric_limits<double>::infinity())),
      m_viscosity(ptree.get<double>("viscosity")),
      m_diffusion(ptree.get<double>("diffusion")),
      m_potential_order(ptree.get<int>("potential.order", 6)),
      m_potential_depth(ptree.get<double>("potential.depth")),
      m_potential_ground_pos(
          ptree.get<double>("potential.groundstate_location")),
      m_potential_cutoff(
          range_from_tolerance(ptree.get<double>("potential.tolerance"),
                               m_potential_ground_pos, m_potential_order))
{
}

double model_parameters::range_from_tolerance(double reltol, double ground_pos,
                                              int order)
{
  if(reltol == 0)
    return std::numeric_limits<double>::infinity();
  double normrange = (std::sqrt(reltol + 1.0) - 1.0) / reltol;
  return std::pow(normrange, 1.0 / order) * ground_pos;
}

double model_parameters::compute_force(double x) const
{
  if(x > m_potential_cutoff)
    return 0;
  return compute_force_nocutoff(x);
}

double model_parameters::compute_force_nocutoff(double x) const
{
  double normdist = m_potential_ground_pos / x;
  double term = std::pow(normdist, m_potential_order);
  double factor = 2 * m_potential_depth * m_potential_order / x;
  return factor * (term * term - term);
}

double model_parameters::compute_potential_nocutoff(double x) const
{
  double normdist = (m_potential_ground_pos / x);
  double term = std::pow(normdist, m_potential_order);
  return m_potential_depth * (term * term - 2 * term);
}

double model_parameters::interaction_range() const
{
  return m_potential_cutoff;
}

double model_parameters::max_jump() const { return m_max_jump; }
double model_parameters::diffusion() const { return m_diffusion; }
double model_parameters::brownian_displacement(std::mt19937_64 &rng,
                                               double t) const
{
  std::normal_distribution<double> dist(0, t * m_diffusion);
  return dist(rng);
}

double model_parameters::viscosus_speed(double dt, double old_speed) const
{
  return std::exp(-m_viscosity * dt) * old_speed;
}

double model_parameters::min_time_step() const { return m_min_time_step; }
double model_parameters::max_time_step() const { return m_max_time_step; }
END_NAMESPACE_ADHESION_2D
