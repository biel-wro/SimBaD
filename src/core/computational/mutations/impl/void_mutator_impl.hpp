#ifndef SIMBAD_VOID_MUTATOR_IMPL_HPP
#define SIMBAD_VOID_MUTATOR_IMPL_HPP
#include <computational/mutations/void_mutator.hpp>

#include "computational/mutations/void_mutator.hpp"

BEGIN_NAMESPACE_CORE
template <class RealType>
void_mutator<RealType>::void_mutator(property_tree const &)
{
}
template <class RealType>
template <class RNG>
RealType void_mutator<RealType>::operator()(real_type v, RNG &) const
{
  return v;
}
template <class RealType>
RealType void_mutator<RealType>::operator()(real_type v,
                                            std::mt19937_64 &rng) const
{
  return this->operator()<std::mt19937_64>(v, rng);
}

END_NAMESPACE_CORE

#endif // SIMBAD_VOID_MUTATOR_IMPL_HPP
