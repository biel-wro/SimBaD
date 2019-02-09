#ifndef SIMBAD_STATELESS_BETA_IMPL_HPP
#define SIMBAD_STATELESS_BETA_IMPL_HPP
#include "computational/mutations/stateless_beta.hpp"
BEGIN_NAMESPACE_CORE
template<class RealType>
stateless_beta<RealType>::stateless_beta(real_type alpha, real_type beta) : m_dist(alpha, beta)
{
}
template<class RealType>
stateless_beta<RealType>::stateless_beta(property_tree const &pt)
    : stateless_beta(pt.get<real_type>("alpha"),
                     pt.get<real_type>("beta"))
{
}
template<class RealType>
template<class RNG>
RealType stateless_beta<RealType>::operator()(real_type, RNG &rng) const {
  real_type u = std::uniform_real_distribution<real_type>()(rng);
  return boost::math::quantile(m_dist, u);
}

template <class RealType>
RealType stateless_beta<RealType>::operator()(real_type v,
                                            std::mt19937_64 &rng) const
{
  return this->operator()<std::mt19937_64>(v, rng);
}

END_NAMESPACE_CORE
#endif //SIMBAD_STATELESS_BETA_IMPL_HPP
