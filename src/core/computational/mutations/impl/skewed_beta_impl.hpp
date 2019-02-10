#ifndef SIMBAD_SKEWED_BETA_IMPL_HPP
#define SIMBAD_SKEWED_BETA_IMPL_HPP

#include "computational/mutations/skewed_beta.hpp"

#include "interface/property_tree.hpp"
#include <boost/math/distributions/beta.hpp>

BEGIN_NAMESPACE_CORE

template<class RealType>
skewed_beta<RealType>::skewed_beta(real_type alpha,
                                   real_type improvement_prob,
                                   real_type scale)
    : m_alpha(alpha), m_impr_prob(improvement_prob), m_scale(scale)
{
}
template<class RealType>
skewed_beta<RealType>::skewed_beta(property_tree const &pt)
    : skewed_beta(pt.get<real_type>("alpha"),
                  pt.get<real_type>("improvement_probability"),
                  pt.get<real_type>("scale"))
{
}
template<class RealType>
RealType skewed_beta<RealType>::find_beta(real_type parent_val) const {
  return boost::math::ibetac_invb(m_alpha, parent_val, m_impr_prob);
}
template<class RealType>
template<class RNG>
RealType skewed_beta<RealType>::operator()(real_type parent_val,
                                            RNG &rng) const {
  real_type beta = find_beta(parent_val);
  real_type u = std::uniform_real_distribution<real_type>()(rng);
  boost::math::beta_distribution<real_type> beta_dist(m_alpha, beta);
  real_type x = boost::math::quantile(beta_dist, u);
  return x;
}

template <class RealType>
RealType skewed_beta<RealType>::operator()(real_type v,
                                            std::mt19937_64 &rng) const
{
  return this->operator()<std::mt19937_64>(v, rng);
}

END_NAMESPACE_CORE

#endif //SIMBAD_SKEWED_BETA_IMPL_HPP
