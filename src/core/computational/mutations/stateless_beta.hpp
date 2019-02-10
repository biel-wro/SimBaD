#ifndef SIMBAD_COMPUTATIONAL_MUTATORS_STATELESS_BETA_MUTATOR_HPP
#define SIMBAD_COMPUTATIONAL_MUTATORS_STATELESS_BETA_MUTATOR_HPP

#include "interface/interface_fwd.hpp"
#include "interface/mutator.hpp"

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
#include "impl/stateless_beta_impl.hpp"

#endif
