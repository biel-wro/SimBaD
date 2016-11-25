#ifndef CORE_GENERALIZED_EXPONENTIAL_INTERACTION
#define CORE_GENERALIZED_EXPONENTIAL_INTERACTION

#include "core_def.hpp"
#include "property_tree.hpp"
#include <cmath>

BEGIN_NAMESPACE_CORE

template <class scalar_type>
static scalar_type compute_cutoff(scalar_type sigma, scalar_type gamma,
                                  scalar_type tolerance)
{
  if(std::numeric_limits<scalar_type>::infinity() == gamma)
    return sigma * sigma;

  scalar_type base = -gamma * std::log(tolerance);
  scalar_type range = std::pow(base, scalar_type(1) / gamma) * sigma;
  return range;
}

template <class ScalarType> class generalized_exponential_interaction
{
public:
  using scalar_type = ScalarType;

  generalized_exponential_interaction(scalar_type sigma, scalar_type gamma = 2,
                                      scalar_type cutoff_tolerance = 0.001)
      : m_gamma(gamma),
        m_factor(scalar_type(-1) / (gamma * std::pow(sigma, gamma))),
        m_cutoff(compute_cutoff(sigma, gamma, cutoff_tolerance))
  {
  }
  generalized_exponential_interaction(property_tree const &pt)
      : generalized_exponential_interaction(pt.get<scalar_type>("sigma"),
                                            pt.get<scalar_type>("gamma"),
                                            pt.get<scalar_type>("tolerance"))
  {
  }
  static generalized_exponential_interaction
  from_property_tree(property_tree const &ptree)
  {
    return generalized_exponential_interaction(ptree);
  }
  scalar_type operator()(scalar_type distance) const
  {
    if(distance <= 0)
      return scalar_type(1);

    if(distance > m_cutoff)
      return scalar_type(0);

    if(std::numeric_limits<scalar_type>::infinity() == m_gamma)
      return distance <= m_cutoff ? scalar_type(1) : scalar_type(0);

    scalar_type exponent = std::pow(distance, m_gamma) * m_factor;
    return std::exp(exponent);
  }

  scalar_type range() const { return m_cutoff; }
  scalar_type range_squared() const { return m_cutoff * m_cutoff; }
  scalar_type get_sigma() const
  {
    if(std::numeric_limits<scalar_type>::infinity() == m_gamma)
      return m_cutoff;
    return std::pow(scalar_type(-1) / (m_factor * m_gamma),
                    scalar_type(1) / m_gamma);
  }
  scalar_type get_gamma() const { return m_gamma; }
  scalar_type get_tolerance() const { return operator()(m_cutoff); }
private:
  scalar_type m_gamma;
  scalar_type m_factor;
  scalar_type m_cutoff;
};

END_NAMESPACE_CORE

#endif
