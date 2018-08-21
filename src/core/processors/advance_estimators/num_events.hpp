#ifndef SIMBAD_PROCESSORS_ADVANCE_ESTIMATORS_NUM_EVENTS_HPP
#define SIMBAD_PROCESSORS_ADVANCE_ESTIMATORS_NUM_EVENTS_HPP

#include "interface/static_advance_estimator.hpp"

#include "advance_estimators_def.hpp"

BEGIN_NAMESPACE_ADVANCE_ESTIMATORS
class num_events final : public static_advance_estimator
{
public:
  explicit num_events(std::size_t start_num_events, std::size_t
  num_events_step = 1);
  explicit num_events(property_tree const &pt);

  std::size_t estimate() const override;
  bool next_target() override;
  void configuration_update(std::size_t events_since_last_update,
                            configuration_view const &) override;

private:
  std::size_t const m_num_events_step;
  std::size_t m_next_target;
  std::size_t m_total_events;
};
END_NAMESPACE_ADVANCE_ESTIMATORS
#endif // SIMBAD_CORE_NUM_EVENTS_ADVANCER_HPP
