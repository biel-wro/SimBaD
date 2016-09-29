#ifndef SATURATION_INTENSITY_HPP
#define SATURATION_INTENSITY_HPP

#include "simple_exp_2d_fwd.hpp"

#include <limits>

#include <boost/property_tree/ptree_fwd.hpp>

BEGIN_NAMESPACE_SIMPLE_EXP_2D
class saturation_intensity
{
public:
  using scalar_type = double;
  explicit saturation_intensity(
      scalar_type sigma,
      scalar_type gamma = std::numeric_limits<scalar_type>::infinity(),
      scalar_type scale = 1);

  static saturation_intensity
  from_property_tree(boost::property_tree::ptree const &ptree);

  scalar_type operator()(scalar_type val) const;

  scalar_type get_gamma() const;
  scalar_type get_sigma() const;
  scalar_type get_scale() const;

private:
  scalar_type m_sigma;
  scalar_type m_gamma;
  scalar_type m_scale;
};

END_NAMESPACE_SIMPLE_EXP_2D

#endif // SATURATION_INTENSITY_HPP
