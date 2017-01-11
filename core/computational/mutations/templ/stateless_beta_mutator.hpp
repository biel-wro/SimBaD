#ifndef STATELESS_BETA_MUTATOR_HPP
#define STATELESS_BETA_MUTATOR_HPP
#include "core_def.hpp"
#include "interface/property_tree.hpp"
#include <boost/math/distributions/beta.hpp>
#include <random>
BEGIN_NAMESPACE_CORE
template <class RealType> class stateless_beta_mutator
{
public:
  using real_type = RealType;
  stateless_beta_mutator(real_type alpha, real_type beta) : m_dist(alpha, beta)
  {
  }
  stateless_beta_mutator(property_tree const &pt)
      : stateless_beta_mutator(pt.get<real_type>("alpha"),
                               pt.get<real_type>("beta"))
  {
  }

  template <class RNG> real_type operator()(real_type, RNG &rng) const
  {
    real_type u = std::uniform_real_distribution<real_type>()(rng);
    return boost::math::quantile(m_dist, u);
  }

private:
  boost::math::beta_distribution<real_type> m_dist;
};
END_NAMESPACE_CORE
#endif // STATELESS_BETA_MUTATOR_HPP
