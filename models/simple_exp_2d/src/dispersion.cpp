#include "dispersion.hpp"
#include <boost/property_tree/ptree.hpp>

BEGIN_NAMESPACE_SIMPLE_EXP_2D


dispersion dispersion::from_property_tree(const boost::property_tree::ptree &ptree){
  scalar_type sigma = ptree.get<scalar_type>("sigma");
  return dispersion(0,sigma);
}

END_NAMESPACE_SIMPLE_EXP_2D
