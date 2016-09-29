#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "simple_exp_2d_fwd.hpp"
#include <cmath>

#include <boost/property_tree/ptree_fwd.hpp>

#include "config.hpp"
#include "simple_exp_2d_def.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D
class kernel
{
public:
  using scalar_type = config::space_coord_scalar;

  kernel(scalar_type sigma, scalar_type gamma = 2,
         scalar_type cutoff_tolerance = 0.001);

  static kernel from_property_tree(boost::property_tree::ptree const &ptree);
  scalar_type operator()(scalar_type distance_squared) const;
  scalar_type operator()(particle const &p1, particle const &p2) const;
  scalar_type range() const;

  scalar_type range_squared() const;
  scalar_type get_sigma() const;
  scalar_type get_gamma() const;

private:
  scalar_type m_power;
  scalar_type m_factor;
  scalar_type m_cutoff_squared;
};

END_NAMESPACE_SIMPLE_EXP_2D

#endif // KERNEL_HPP
