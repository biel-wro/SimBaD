#ifndef NVOID_MUTATOR_HPP
#define NVOID_MUTATOR_HPP

#include "interface/mutator.hpp"

BEGIN_NAMESPACE_CORE
template <class RealType> class void_mutator final: public mutator<RealType>
{
  using real_type = RealType;
public:
  void_mutator(property_tree const &);
  template <class RNG> real_type operator()(real_type v, RNG &) const;
  real_type operator()(real_type, std::mt19937_64 &) const override;
};

END_NAMESPACE_CORE

#endif // NVOID_MUTATOR_HPP
