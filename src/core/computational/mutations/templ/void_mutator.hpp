#ifndef NVOID_MUTATOR_HPP
#define NVOID_MUTATOR_HPP

#include "interface/interface_fwd.hpp"


BEGIN_NAMESPACE_CORE
template <class RealType> class void_mutator
{
  using real_type = RealType;

public:
  void_mutator(property_tree const &) {}
  template <class RNG> real_type operator()(real_type v, RNG &) const
  {
    return v;
  }
};
END_NAMESPACE_CORE

#endif // NVOID_MUTATOR_HPP
