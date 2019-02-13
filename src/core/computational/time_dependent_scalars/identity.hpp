#ifndef SIMBAD_CORE_COMPUTATIONAL_TIMEDEP_IDENTITY_HPP
#define SIMBAD_CORE_COMPUTATIONAL_TIMEDEP_IDENTITY_HPP

#include "time_dependent_scalar.hpp"

#include "core_fwd.hpp"

BEGIN_NAMESPACE_CORE

class identity final : public time_dependent_scalar
{
public:
  explicit identity();
  explicit identity(property_tree const &);
  real_type operator()(real_type t, real_type v) const override;
};

END_NAMESPACE_CORE

#endif