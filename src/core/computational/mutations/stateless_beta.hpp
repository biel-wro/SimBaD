#ifndef STATELESS_BETA_MUTATOR_HPP
#define STATELESS_BETA_MUTATOR_HPP

#include "interface/mutator.hpp"

#include "interface/property_tree.hpp"

#include <boost/math/distributions/beta.hpp>

#include <random>

BEGIN_NAMESPACE_CORE
template <class RealType> class stateless_beta final : public mutator<RealType>
{
public:
  using real_type = RealType;
  stateless_beta(real_type alpha, real_type beta);
  explicit stateless_beta(property_tree const &pt);

  template <class RNG> real_type operator()(real_type, RNG &rng) const;
  real_type operator()(real_type, std::mt19937_64 &rng) const override;

private:
  boost::math::beta_distribution<real_type> m_dist;
};
END_NAMESPACE_CORE
#endif // STATELESS_BETA_MUTATOR_HPP
