#ifndef CORE_EXTRACTOR_HPP
#define CORE_EXTRACTOR_HPP

#include "interface/interface_fwd.hpp"
#include <random>

BEGIN_NAMESPACE_CORE

template<class Real>
class extractor
{
public:
  using real_type = Real;
  virtual real_type operator()(real_type val) const = 0;
  virtual ~extractor();
};



END_NAMESPACE_CORE
#endif // MUTATOR_HPP