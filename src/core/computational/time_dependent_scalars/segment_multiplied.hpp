#ifndef SIMBAD_COMPUTATIONAL_TIME_DEP_SEGMENT_MULTIPLED_HPP
#define SIMBAD_COMPUTATIONAL_TIME_DEP_SEGMENT_MULTIPLED_HPP

#include "time_dependent_scalar.hpp"
#include "interface/fwd/properties_fwd.hpp"

BEGIN_NAMESPACE_CORE

class segment_multiplied : public time_dependent_scalar{
public:
  segment_multiplied(real_type start_time, real_type end_time, real_type factor);
  explicit segment_multiplied(property_tree const &pt);

  real_type operator()(real_type time, real_type value) const override;
private:
  real_type const m_start_time;
  real_type const m_end_time;
  real_type const m_factor;

};
END_NAMESPACE_CORE
#endif
