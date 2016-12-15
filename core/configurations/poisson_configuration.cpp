#include "configurations/poisson_configuration.hpp"
#include "interface/particle.hpp"

#include <cmath>
#include <numeric>
BEGIN_NAMESPACE_CORE

namespace
{
struct particle_view : public particle
{
  explicit particle_view(std::size_t dimension) : m_coords(dimension) {}
  //std::size_t dimension() const override { return m_coords.size(); }
  double coord(std::size_t d) const override
  {
   // assert(d < dimension());
    return m_coords[d];
  }
  void sample_ball(double radius, std::mt19937_64 &rng)
  {
    double (*hypot)(double, double) = std::hypot;
    do
    {
      sample_cube(radius, rng);
    } while(radius <
            std::accumulate(m_coords.begin(), m_coords.end(), 0.0, hypot));
  }
  void sample_cube(double halflen, std::mt19937_64 &rng)
  {
    for(double &c : m_coords)
      c = std::uniform_real_distribution<double>(-halflen, halflen)(rng);
  }

private:
  std::vector<double> m_coords;
};
}
poisson_configuration::poisson_configuration(const property_tree &pt)
    : m_radius(pt.get<double>("radius")),
      m_dimension(pt.get<dimension_type>("dimension")),
      m_rng(pt.get<std::uint64_t>("seed")),
      m_is_ball(pt.get<bool>("ball", false))
{
  boost::optional<double> lambda = pt.get_optional<double>("lambda");
  if(lambda)
    set_sample_size(lambda.get());
  else
    set_size(pt.get<size_type>("size"));
}
poisson_configuration::poisson_configuration(dimension_type dimension,
                                             size_type n, double radius,
                                             bool ball, std::uint64_t seed)
    : m_radius(radius),
      m_dimension(dimension),
      m_size(n),
      m_rng(seed),
      m_is_ball(ball)
{
}

poisson_configuration::poisson_configuration(
    poisson_configuration::dimension_type dimension, double lambda,
    double radius, bool ball, std::uint64_t seed)
    : m_radius(radius), m_dimension(dimension), m_rng(seed), m_is_ball(ball)
{
  set_sample_size(lambda);
}

poisson_configuration::size_type
poisson_configuration::configuration_size() const
{
  return m_size;
}

configuration_view::size_type poisson_configuration::dimension() const
{
  return m_dimension;
}

void poisson_configuration::visit_configuration(
    configuration_view::particle_visitor v) const
{
  using sampler_ptr = void (particle_view::*)(double, std::mt19937_64 &);

  sampler_ptr sampler;
  if(m_is_ball)
    sampler = &particle_view::sample_ball;
  else
    sampler = &particle_view::sample_cube;

  particle_view p(m_dimension);
  std::mt19937_64 rng = m_rng;
  for(size_type i = 0; i < m_size; ++i)
  {
    (p.*sampler)(m_radius, rng);
    v(p);
  }
}

void poisson_configuration::set_dimension(
    poisson_configuration::dimension_type d)
{
  m_dimension = d;
}

void poisson_configuration::set_size(poisson_configuration::size_type n)
{
  m_size = n;
}

void poisson_configuration::set_sample_size(double lambda)
{
  m_size = sample_poisson(lambda);
}

poisson_configuration::size_type
poisson_configuration::sample_poisson(double lambda)
{
  return std::poisson_distribution<size_type>(lambda)(m_rng);
}

END_NAMESPACE_CORE
