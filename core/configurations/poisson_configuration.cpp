#include "configurations/poisson_configuration.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "interface/particle.hpp"

#include <cmath>
#include <numeric>
BEGIN_NAMESPACE_CORE

namespace
{
struct particle_view : public attribute_list
{
  explicit particle_view(std::size_t dimension) : m_coords(dimension) {}
  attribute get_attribute(std::size_t attributeno) const
  {
    assert(0 == attributeno);
    return m_coords;
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
  attribute_array<double> m_coords;
};
}
poisson_configuration::poisson_configuration(const property_tree &pt)
    : poisson_configuration(
          pt.get<dimension_type>("dimension"), pt.get<double>("radius"),
          pt.get<bool>("ball", false), pt.get<std::uint64_t>("seed"))
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
      m_rng(seed),
      m_size(n),
      m_is_ball(ball),
      m_attr_desc(new attribute_descriptor)
{
  m_attr_desc->add_attribute(0, "position", ATTRIBUTE_KIND::POSITION,
                             dimension);
}

poisson_configuration::poisson_configuration(
    poisson_configuration::dimension_type dimension, double lambda,
    double radius, bool ball, std::uint64_t seed)
    : poisson_configuration(dimension, sample_poisson(lambda), radius, ball,
                            seed)
{
}

poisson_configuration::size_type
poisson_configuration::size() const
{
  return m_size;
}

void poisson_configuration::visit_records(
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

const attribute_descriptor &poisson_configuration::descriptor() const
{
  return attribute_descriptor::make_position_only();
}

poisson_configuration::~poisson_configuration() {}
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
