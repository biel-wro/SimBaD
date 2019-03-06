#ifndef SIMBAD_CORE_COMPUTATIONAL_TIME_SCALARS_TEETH_HPP
#define SIMBAD_CORE_COMPUTATIONAL_TIME_SCALARS_TEETH_HPP

#include "time_dependent_scalar.hpp"
#include "interface/fwd/properties_fwd.hpp"

BEGIN_NAMESPACE_CORE

class teeth : public time_dependent_scalar
{
public:
  teeth(real_type start_time, real_type end_time, real_type factor, real_type
  on_period,
        real_type off_period);
  explicit teeth(property_tree const &pt);

  virtual real_type operator()(real_type time, real_type value) const override;
  virtual ~teeth();

private:
  real_type m_start_time;
  real_type m_end_time;
  real_type m_factor;
  real_type m_on_period;
  real_type m_off_period;
};

END_NAMESPACE_CORE

#endif // SIMBAD_TEETH_HPP
