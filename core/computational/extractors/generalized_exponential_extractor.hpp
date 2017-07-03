#ifndef CORE_GENERALIZED_EXPONENTIAL_EXTRACTOR_HPP
#define CORE_GENERALIZED_EXPONENTIAL_EXTRACTOR_HPP
#include "core_def.hpp"
#include "interface/property_tree.hpp"

#include <cmath>
#include <limits>

BEGIN_NAMESPACE_CORE

template <class Scalar> class generalized_exponential_extractor
{
public:
  using scalar_type = Scalar;
  explicit generalized_exponential_extractor(property_tree const &pt)
      : m_sigma(pt.get<scalar_type>("sigma")),
        m_gamma(pt.get<scalar_type>("gamma")),
        m_scale(pt.get<scalar_type>("scale"))
  {
  }

  explicit generalized_exponential_extractor(
      scalar_type sigma = 1,
      scalar_type gamma = std::numeric_limits<scalar_type>::infinity(),
      scalar_type scale = 1)
      : m_sigma(sigma), m_gamma(gamma), m_scale(scale)
  {
  }

  static generalized_exponential_extractor
  from_property_tree(property_tree const &ptree)
  {
    return generalized_exponential_extractor(ptree);
  }

  scalar_type operator()(scalar_type val) const
  {
    if(std::numeric_limits<scalar_type>::infinity() == m_gamma)
      return val <= m_sigma ? m_scale : 0;
    scalar_type exponent = -std::pow(val / m_sigma, m_gamma) / m_gamma;
    return m_scale * std::exp(exponent);
  }

  scalar_type get_gamma() const { return m_gamma; }
  scalar_type get_sigma() const { return m_sigma; }
  scalar_type get_scale() const { return m_scale; }
  void set_gamma(scalar_type gamma) { m_gamma = gamma; }
  void set_sigma(scalar_type sigma) { m_sigma = sigma; }
  void set_scale(scalar_type scale) { m_scale = scale; }

private:
  scalar_type m_sigma;
  scalar_type m_gamma;
  scalar_type m_scale;
};

END_NAMESPACE_CORE
#endif // GENERALIZED_EXPONENTIAL_EXTRACTOR_HPP
