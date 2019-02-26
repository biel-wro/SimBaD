#include "segment_multiplied.hpp"
#include "interface/property_tree.hpp"
#include <stdexcept>
BEGIN_NAMESPACE_CORE

segment_multiplied::segment_multiplied(real_type start_time, real_type end_time,
                                       real_type factor)
    : m_start_time(start_time), m_end_time(end_time), m_factor(factor)
{
  if(end_time < start_time)
    throw std::logic_error("start_time must be lower than end_time");
}

segment_multiplied::segment_multiplied(const simbad::core::property_tree &pt)
    : segment_multiplied(pt.get<real_type>("start_time"),
                         pt.get<real_type>("end_time"),
                         pt.get<real_type>("factor"))
{
}

segment_multiplied::real_type segment_multiplied::
operator()(real_type time, real_type value) const
{
  if(m_start_time < time && time < m_end_time)
    return value * m_factor;
  return value;
}
END_NAMESPACE_CORE