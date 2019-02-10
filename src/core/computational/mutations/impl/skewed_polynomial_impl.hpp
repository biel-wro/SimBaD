#ifndef SIMBAD_SKEWED_POLYNOMIAL_IMPL_HPP
#define SIMBAD_SKEWED_POLYNOMIAL_IMPL_HPP
#include "computational/mutations/skewed_polynomial.hpp"
#include "interface/property_tree.hpp"
BEGIN_NAMESPACE_CORE

template<class RealType>
skewed_polynomial<RealType>::skewed_polynomial(real_type improvement_prob,
                                               real_type improvement_power,
                                               real_type deterioration_power)
    : m_impr_prob(improvement_prob),
      m_improvement_exponent(improvement_power),
      m_deterioraiton_exponent(deterioration_power)
{
}
template<class RealType>
skewed_polynomial<RealType>::skewed_polynomial(property_tree const &pt)
    : skewed_polynomial(pt.get<real_type>("improvement_probability"),
                        pt.get<real_type>("improvement_exponent"),
                        pt.get<real_type>("deterioration_exponent"))
{
}
template<class RealType>
template<class RNG>
RealType skewed_polynomial<RealType>::operator()(real_type parent_val,
                                                  RNG &rng) const {
  real_type u = std::uniform_real_distribution<real_type>()(rng);

  if(u < m_impr_prob)
    return improve(parent_val, rng);
  return hinder(parent_val, rng);
}
template<class RealType>
template<class RNG>
RealType skewed_polynomial<RealType>::improve(real_type parent_val,
                                               RNG &rng) const {
  real_type r = polyrnd_inv(m_improvement_exponent, rng);
  real_type increase = r * (max_value() - parent_val);
  return std::min(parent_val + increase, max_value());
}
template<class RealType>
template<class RNG>
RealType skewed_polynomial<RealType>::hinder(real_type parent_val,
                                              RNG &rng) const {
  real_type r = polyrnd_inv(m_deterioraiton_exponent, rng);
  real_type decrease = r * (parent_val - min_value());
  return std::max(parent_val - decrease, min_value());
}
template<class RealType>
template<class RNG>
RealType skewed_polynomial<RealType>::polyrnd(real_type power, RNG &rng) const {
  real_type q = std::uniform_real_distribution<real_type>()(rng);
  return std::pow(q, 1 / (power + 1));
}
template<class RealType>
template<class RNG>
RealType skewed_polynomial<RealType>::polyrnd_inv(real_type power,
                                                   RNG &rng) const {
  return 1 - polyrnd(power, rng);
}

template <class RealType>
RealType skewed_polynomial<RealType>::operator()(real_type v,
                                            std::mt19937_64 &rng) const
{
  return this->operator()<std::mt19937_64>(v, rng);
}

END_NAMESPACE_CORE

#endif //SIMBAD_SKEWED_POLYNOMIAL_IMPL_HPP
