#include "population_size.hpp"

#include "interface/property_tree.hpp"

BEGIN_NAMESPACE_ADVANCE_ESTIMATORS
population_size::population_size(std::size_t initial,
                                                   std::size_t step)
    : m_next_target(initial), m_target_step(step), m_target_achieved(false)
{
}
population_size::population_size(property_tree const &pt)
    : population_size(pt.get("start_size", std::size_t(0)),
                               pt.get("size_step", std::size_t(0)))
{
}
std::size_t population_size::estimate() const
{
  if(m_target_achieved)
    return 0;
  if(m_next_target < 100)
    return 1;
  if(m_next_target < 1000)
    return 10;
  if(m_next_target < 10000)
    return 100;
  if(m_next_target < 100000)
    return 1000;
  return 10000;
}
bool population_size::next_target()
{
  m_next_target += m_target_step;
  m_target_achieved = false;
  return true;
}
void population_size::configuration_update(
    std::size_t, configuration_view const &conf)
{
  std::size_t current_size = conf.size();
  if(current_size >= m_next_target)
    m_target_achieved = true;
}
END_NAMESPACE_ADVANCE_ESTIMATORS