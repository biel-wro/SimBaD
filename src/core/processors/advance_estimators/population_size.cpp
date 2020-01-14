#include "population_size.hpp"

#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_ADVANCE_ESTIMATORS
population_size::population_size(std::size_t initial, std::size_t step)
    : m_next_target(initial),
      m_target_step(step),
      m_last_observed(0),
      m_prev_target(0)
{
}
population_size::population_size(property_tree const &pt)
    : population_size(pt.get("start_size", std::size_t(0)),
                      pt.get("size_step", std::size_t(0)))
{
}
std::size_t population_size::estimate() const
{
  if(m_next_target <= m_last_observed)
    return 0;

  std::size_t population_yet_missing = m_next_target - m_last_observed;

  if(population_yet_missing < 100)
    return 1;
  if(population_yet_missing < 1000)
    return 10;
  if(population_yet_missing < 10000)
    return 100;
  if(population_yet_missing < 100000)
    return 1000;
  return 10000;
}

float population_size::progress() const
{
  std::size_t cycle_pos = m_next_target - m_last_observed;

  if(cycle_pos > m_next_target) // last observation is beyond target
    return 1.0f;

  std::size_t cycle_size = m_next_target - m_prev_target;

  return float(double(cycle_pos)/cycle_size);
}

bool population_size::next_target()
{
  m_prev_target = m_next_target;
  m_next_target += m_target_step;
  return true;
}
void population_size::initialize(std::size_t, configuration_view const &conf)
{
  m_last_observed = conf.size();
}

void population_size::configuration_update(std::size_t,
                                           configuration_view const &conf)
{
  m_last_observed = conf.size();
}
END_NAMESPACE_ADVANCE_ESTIMATORS