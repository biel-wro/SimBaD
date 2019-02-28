#include "in_simulation_time.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_kind.hpp"
#include "interface/attribute_list.hpp"
#include "interface/event_source.hpp"
#include "interface/property_tree.hpp"

#include <cmath>
#include <limits>

BEGIN_NAMESPACE_ADVANCE_ESTIMATORS

in_simulation_time::in_simulation_time(double time_step, double start_time)
    : m_time_step(time_step),
      m_start_time(start_time),
      m_num_step(0),
      m_target_time(start_time),

      m_last_observed_time(-std::numeric_limits<double>::infinity()),
      m_delta_time(0),
      m_speed(std::numeric_limits<double>::quiet_NaN())
{
}
in_simulation_time::in_simulation_time(const simbad::core::property_tree &pt)
    : in_simulation_time(pt.get("time_step", 1.0), pt.get("start_time", 0.0))
{
}
std::size_t in_simulation_time::estimate() const
{
  // do not have an observation yet
  if(-std::numeric_limits<double>::infinity() == m_last_observed_time)
    return 1;

  // target reached
  if(m_target_time <= m_last_observed_time)
    return 0;

  double time_until_target = m_target_time - m_last_observed_time;

  if(!std::isfinite(time_until_target))
    return 1;

  if(m_delta_time * 100 < time_until_target )
    return 1;

  double linear_estimate = m_speed * time_until_target;
  std::size_t proposed_step = linear_estimate * 0.01;

  return std::min(std::size_t(10000), std::max(std::size_t(1), proposed_step));
}
bool in_simulation_time::next_target()
{
  m_num_step += 1;
  m_target_time = m_start_time + m_num_step * m_time_step;

  return true;
}
void in_simulation_time::set_description(
    attribute_description const &description)
{
  m_time_attribute_idx =
      description.get_descriptor(ATTRIBUTE_KIND::EVENT_TIME, true)
          .value()
          .attribute_idx();
}

void in_simulation_time::event_update(std::size_t events_since_last_update,
                                      event const &e)
{
  double const current_time = e[m_time_attribute_idx].get_real_val();

  m_delta_time = current_time - m_last_observed_time;

  m_speed = events_since_last_update / m_delta_time;

  m_last_observed_time = current_time;
}

END_NAMESPACE_ADVANCE_ESTIMATORS