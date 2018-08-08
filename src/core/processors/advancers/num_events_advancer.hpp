#ifndef SIMBAD_CORE_NUM_EVENTS_ADVANCER_HPP
#define SIMBAD_CORE_NUM_EVENTS_ADVANCER_HPP

#include "interface/advancer.hpp"


BEGIN_NAMESPACE_CORE
class num_events_advancer final : public advancer
{
public:
  explicit num_events_advancer(std::size_t num_events_step = 1);
  explicit num_events_advancer(property_tree const &pt);

  std::size_t estimate() const override;
  bool next_target() override;

private:
  std::size_t const m_num_events_step;
  std::size_t m_next_target;
};
END_NAMESPACE_CORE
#endif // SIMBAD_CORE_NUM_EVENTS_ADVANCER_HPP
