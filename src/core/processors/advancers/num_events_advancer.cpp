#include "num_events_advancer.hpp"

#include "interface/event_source.hpp"
#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_CORE
num_events_advancer::num_events_advancer(size_t num_events_step)
    : m_num_events_step(num_events_step), m_next_target(0)
{
}

num_events_advancer::num_events_advancer(property_tree const &pt)
    : num_events_advancer(pt.get("num_events", std::size_t(1)))
{
}

std::size_t num_events_advancer::estimate() const { return m_next_target; }

bool num_events_advancer::next_target()
{
  m_next_target += m_num_events_step;
  return true;
}
END_NAMESPACE_CORE