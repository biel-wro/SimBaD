#ifndef CORE_GENERALIZED_EXPONENTIAL_INTERACTION
#define CORE_GENERALIZED_EXPONENTIAL_INTERACTION

#include "core_def.hpp"
#include "property_tree.hpp"
#include <cmath>

BEGIN_NAMESPACE_CORE
class generalized_exponential_interaction
{
public:
  using scalar_type = config::space_coord_scalar;

  generalized_exponential_interaction(scalar_type sigma, scalar_type gamma = 2,
         scalar_type cutoff_tolerance = 0.001)
      : m_power(gamma / 2.f),
        m_factor(-1.f / (gamma * std::pow(sigma, gamma))),
        m_cutoff_squared(compute_cutoff_squared(sigma, gamma, cutoff_tolerance))
  {
  }

  static generalized_exponential_interaction from_property_tree(property_tree const &ptree)
  {
    scalar_type sigma = ptree.get<scalar_type>("sigma");
    scalar_type gamma = ptree.get<scalar_type>("gamma");
    scalar_type tolerance = ptree.get<scalar_type>("tolerance");
    return generalized_exponential_interaction(sigma, gamma, tolerance);
  }
  scalar_type operator()(scalar_type distance_squared) const
  {
    if(distance_squared <= 0)
      return 1;

    if(distance_squared > m_cutoff_squared)
      return 0;

    if(std::numeric_limits<scalar_type>::infinity() == m_power)
      return distance_squared <= m_cutoff_squared ? 1 : 0;

    scalar_type exponent = std::pow(distance_squared, m_power) * m_factor;
    return std::exp(exponent);
  }
  scalar_type operator()(particle const &p1, particle const &p2) const
  {
    scalar_type distance_square = p1.coords().distance_square_to(p2.coords());
    return operator()(distance_square);
  }
  scalar_type range() const
  {
    return std::sqrt(m_cutoff_squared);
  }

  scalar_type range_squared() const { return m_cutoff_squared; }
  scalar_type get_sigma() const
  {
    if(std::numeric_limits<scalar_type>::infinity() == m_power)
      return std::sqrt(m_cutoff_squared);
    scalar_type gamma = get_gamma();
    return std::pow(-m_factor, -1.f / gamma) / std::pow(gamma, 1.f / gamma);
  }
  scalar_type get_gamma() const { return m_power * 2; }

private:
  scalar_type m_power;
  scalar_type m_factor;
  scalar_type m_cutoff_squared;
};

END_NAMESPACE_CORE

#endif
