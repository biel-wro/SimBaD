#include "in_simulation_time_advancer.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_kind.hpp"
#include "interface/attribute_list.hpp"
#include "interface/event_source.hpp"
#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_CORE

in_simulation_time_advancer::in_simulation_time_advancer(double time_step,
                                                         double start_time)
    : m_time_step(time_step),
      m_start_time(start_time),
      m_num_step(0),
      m_target_time(start_time),
      // dumb imlementation
      m_target_achieved(true)
{
}
in_simulation_time_advancer::in_simulation_time_advancer(
    const simbad::core::property_tree &pt)
    : in_simulation_time_advancer(pt.get("time_step", 1.0),
                                  pt.get("start_time", 0.0))
{
}
std::size_t in_simulation_time_advancer::estimate() const
{
  if(m_target_achieved)
    return 0;

  if(m_target_time <= 5.0)
    return 1;
  if(m_target_time <= 10.0)
    return 100;
  return 1000;
}
bool in_simulation_time_advancer::next_target()
{
  m_num_step += 1;
  m_target_time = m_start_time + m_num_step * m_time_step;

  m_target_achieved = false;
  return true;
}
void in_simulation_time_advancer::set_description(
    attribute_description const &description)
{
  m_time_attribute_idx =
      description.get_descriptor(ATTRIBUTE_KIND::TIME).get().attribute_idx();
}
void in_simulation_time_advancer::read_event(std::size_t, event const &e)
{
  double const current_time = e[m_time_attribute_idx].get_real_val();
  if(current_time >= m_target_time)
    m_target_achieved = true;
}

END_NAMESPACE_CORE