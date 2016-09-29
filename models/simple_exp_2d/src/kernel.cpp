#include "kernel.hpp"

#include <boost/property_tree/ptree.hpp>

#include "particle_2d.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D

template <class scalar_type>
static scalar_type compute_cutoff_squared(scalar_type sigma, scalar_type gamma,
                                          scalar_type tolerance)
{
  if (std::numeric_limits<scalar_type>::infinity() == gamma)
    return sigma * sigma;

  scalar_type base = -gamma * std::log(tolerance);
  scalar_type range = std::pow(base, 1.f / gamma) * sigma;
  return range * range;
}

kernel::kernel(kernel::scalar_type sigma, kernel::scalar_type gamma,
               scalar_type cutoff_tolerance)
    : m_power(gamma / 2.f),
      m_factor(-1.f / (gamma * std::pow(sigma, gamma))),
      m_cutoff_squared(compute_cutoff_squared(sigma, gamma, cutoff_tolerance))
{
}

kernel kernel::from_property_tree(const boost::property_tree::ptree &ptree) {

  scalar_type sigma = ptree.get<scalar_type>("sigma");
  scalar_type gamma = ptree.get<scalar_type>("gamma");
  scalar_type tolerance = ptree.get<scalar_type>("tolerance");
  return kernel(sigma,gamma,tolerance);
}

kernel::scalar_type kernel::
operator()(kernel::scalar_type distance_squared) const
{
  if (distance_squared <= 0)
    return 1;

  if (distance_squared > m_cutoff_squared)
    return 0;

  if (std::numeric_limits<scalar_type>::infinity() == m_power)
    return distance_squared <= m_cutoff_squared ? 1 : 0;

  scalar_type exponent = std::pow(distance_squared, m_power) * m_factor;
  return std::exp(exponent);
}

kernel::scalar_type kernel::operator()(const particle &p1,
                                       const particle &p2) const
{
  scalar_type distance_square = p1.coords().distance_square_to(p2.coords());
  return operator()(distance_square);
}

kernel::scalar_type kernel::range() const
{
  return std::sqrt(m_cutoff_squared);
}

kernel::scalar_type kernel::range_squared() const { return m_cutoff_squared; }

kernel::scalar_type kernel::get_sigma() const
{
  if (std::numeric_limits<scalar_type>::infinity() == m_power)
    return std::sqrt(m_cutoff_squared);
  scalar_type gamma = get_gamma();
  return std::pow(-m_factor, -1.f / gamma) / std::pow(gamma, 1.f / gamma);
}

kernel::scalar_type kernel::get_gamma() const { return m_power * 2; }

END_NAMESPACE_SIMPLE_EXP_2D
