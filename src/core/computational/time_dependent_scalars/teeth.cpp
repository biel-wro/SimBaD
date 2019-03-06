#include "teeth.hpp"

#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_CORE

teeth::teeth(real_type start_time, real_type end_time, real_type factor,
             real_type on_period, real_type off_period)
    : m_start_time(start_time),
      m_end_time(end_time),
      m_factor(factor),
      m_on_period(on_period),
      m_off_period(off_period)
{
}

teeth::teeth(property_tree const &pt)
    : teeth(pt.get<real_type>("start_time"), pt.get<real_type>("end_time"),
            pt.get<real_type>("factor"), pt.get<real_type>("on_period"),
            pt.get<real_type>("off_period"))
{
}
teeth::real_type teeth::operator()(real_type time, real_type value) const
{
  if(time < m_start_time || m_end_time < time)
    return value;

  real_type t = time - m_start_time;
  real_type period = m_on_period + m_off_period;
  real_type phase = std::fmod(t, period);
  return phase <= m_on_period ? value * m_factor : value;
}

teeth::~teeth() = default;

END_NAMESPACE_CORE