#include "num_events.hpp"

#include "interface/event_source.hpp"
#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_ADVANCE_ESTIMATORS
num_events::num_events(std::size_t start_num_events,
                       std::size_t num_events_step)
    : m_num_events_step(num_events_step),
      m_next_target(start_num_events),
      m_total_events(0)
{
}

num_events::num_events(property_tree const &pt)
    : num_events(pt.get("start", std::size_t(0)),
                 pt.get("step", std::size_t(1)))
{
}

std::size_t num_events::estimate() const
{
  if(m_next_target > m_total_events)
    return m_next_target - m_total_events;
  return 0;
}

bool num_events::next_target()
{
  m_next_target += m_num_events_step;
  return true;
}
void num_events::initialize(std::size_t initial_events,
                            configuration_view const &)
{
  m_total_events = initial_events;
}
void num_events::configuration_update(std::size_t events_since_last_update,
                                      configuration_view const &)
{
  m_total_events += events_since_last_update;
}
END_NAMESPACE_ADVANCE_ESTIMATORS