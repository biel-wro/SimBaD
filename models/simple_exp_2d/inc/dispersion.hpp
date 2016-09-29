#ifndef DISPERSION_HPP
#define DISPERSION_HPP
#include "simple_exp_2d_def.hpp"

#include <random>
#include <boost/property_tree/ptree_fwd.hpp>

BEGIN_NAMESPACE_SIMPLE_EXP_2D
class dispersion : public std::normal_distribution<float>
{
public:
  using scalar_type = float;
  using super_type = std::normal_distribution<scalar_type>;
  using super_type::super_type;


  static dispersion from_property_tree(boost::property_tree::ptree const &ptree);
};
END_NAMESPACE_SIMPLE_EXP_2D
#endif // DISPERSION_HPP
