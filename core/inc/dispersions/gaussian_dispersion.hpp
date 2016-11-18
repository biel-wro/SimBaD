#ifndef GAUSSIAN_DISPERSION_HPP
#define GAUSSIAN_DISPERSION_HPP
#include "core_def.hpp"
#include "property_tree.hpp"

#include <random>

BEGIN_NAMESPACE_CORE
template<class ScalarType>
class dispersion : public std::normal_distribution<float>
{
public:
  using scalar_type = ScalarType;
  using super_type = std::normal_distribution<scalar_type>;
  using super_type::super_type;

  static dispersion from_property_tree(property_tree &ptree)
  {
    scalar_type sigma = ptree.get<scalar_type>("sigma");
    return dispersion(0, sigma);
  }
};
END_NAMESPACE_CORE
#endif // GAUSSIAN_DISPERSION_HPP
