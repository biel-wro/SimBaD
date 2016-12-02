#ifndef GAUSSIAN_DISPERSION_HPP
#define GAUSSIAN_DISPERSION_HPP
#include "core_def.hpp"
#include "property_tree.hpp"

#include <random>

BEGIN_NAMESPACE_CORE
template <class ScalarType>
class gaussian_dispersion //: public std::normal_distribution<ScalarType>
{
public:
  using scalar_type = ScalarType;
  using base_distribuiton = std::normal_distribution<scalar_type>;
  gaussian_dispersion(property_tree const &pt)
      : m_sigma(pt.get<scalar_type>("sigma"))
  {
  }
  template <class RNG> scalar_type operator()(RNG &rng) const
  {
    return base_distribuiton(0, m_sigma)(rng);
  }
  static gaussian_dispersion from_property_tree(property_tree const &pt)
  {
    return gaussian_dispersion(pt);
  }

private:
  scalar_type m_sigma;
};
END_NAMESPACE_CORE
#endif // GAUSSIAN_DISPERSION_HPP
