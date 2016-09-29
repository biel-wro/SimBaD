#include "saturation_intensity.hpp"
#include <boost/property_tree/ptree.hpp>
#include <cmath>

BEGIN_NAMESPACE_SIMPLE_EXP_2D
saturation_intensity::saturation_intensity(scalar_type sigma, scalar_type gamma,
                                           scalar_type scale)
    : m_sigma(sigma), m_gamma(gamma), m_scale(scale)
{
}

saturation_intensity saturation_intensity::from_property_tree(
    const boost::property_tree::ptree &ptree)
{
  scalar_type sigma = ptree.get<scalar_type>("sigma");
  scalar_type gamma = ptree.get<scalar_type>("gamma");
  scalar_type scale = ptree.get<scalar_type>("scale");

  return saturation_intensity(sigma, gamma, scale);
}

saturation_intensity::scalar_type saturation_intensity::operator()(
    simbad::models::simple_exp_2d::saturation_intensity::scalar_type val) const
{
  if (std::numeric_limits<scalar_type>::infinity() == m_gamma)
    return val <= m_sigma ? m_scale : 0;

  scalar_type exponent = -std::pow(val / m_sigma, m_gamma) / m_gamma;
  return m_scale * std::exp(exponent);
}

saturation_intensity::scalar_type saturation_intensity::get_gamma() const
{
  return m_gamma;
}

saturation_intensity::scalar_type saturation_intensity::get_sigma() const
{
  return m_sigma;
}

saturation_intensity::scalar_type saturation_intensity::get_scale() const
{
  return m_scale;
}
END_NAMESPACE_SIMPLE_EXP_2D
