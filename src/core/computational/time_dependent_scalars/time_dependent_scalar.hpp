#ifndef CORE_COMPUTATIONAL_TIME_DEPENDENT_SCALAR_HPP
#define CORE_COMPUTATIONAL_TIME_DEPENDENT_SCALAR_HPP

#include "core_def.hpp"

BEGIN_NAMESPACE_CORE

class time_dependent_scalar
{
public:
  using real_type = double;
  virtual real_type operator()(real_type time, real_type value) const = 0;
  virtual ~time_dependent_scalar();
};

END_NAMESPACE_CORE

#endif
