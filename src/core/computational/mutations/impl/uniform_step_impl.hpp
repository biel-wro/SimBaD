#ifndef SIMBAD_UNIFORM_STEP_IMPL_HPP
#define SIMBAD_UNIFORM_STEP_IMPL_HPP
#include "computational/mutations/uniform_step.hpp"
BEGIN_NAMESPACE_CORE
template<class RealType>
uniform_step<RealType>::uniform_step(real_type increase_length,
                                     real_type decrease_length)
    : m_increase_length(increase_length), m_decrease_length(decrease_length)
{
}
template<class RealType>
uniform_step<RealType>::uniform_step(property_tree const &pt)
    : uniform_step(pt.get<real_type>("increase_length"),
                   pt.get<real_type>("decrease_length"))
{
}
template<class RealType>
template<class RNG>
RealType uniform_step<RealType>::operator()(real_type parent_val,
                                             RNG &rng) const {
  real_type min_val = std::max(real_type(0), parent_val - m_decrease_length);
  real_type max_val = std::min(real_type(1), parent_val + m_increase_length);
  return std::uniform_real_distribution<real_type>(min_val, max_val)(rng);
}

template <class RealType>
RealType uniform_step<RealType>::operator()(real_type v,
                                            std::mt19937_64 &rng) const
{
  return this->operator()<std::mt19937_64>(v, rng);
}
END_NAMESPACE_CORE
#endif //SIMBAD_UNIFORM_STEP_IMPL_HPP
