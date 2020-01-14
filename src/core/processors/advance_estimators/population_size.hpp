#ifndef SIMBAD_PROCESSORS_ADVANCE_ESTIMATORS_POPULATION_SIZE_HPP
#define SIMBAD_PROCESSORS_ADVANCE_ESTIMATORS_POPULATION_SIZE_HPP

#include "advance_estimators_def.hpp"

#include "interface/static_advance_estimator.hpp"

BEGIN_NAMESPACE_ADVANCE_ESTIMATORS
class population_size final : public static_advance_estimator
{
public:
  explicit population_size(std::size_t initial, std::size_t step);
  explicit population_size(property_tree const &);

  std::size_t estimate() const override;
  float progress() const override;
  bool next_target() override;

  void initialize(std::size_t initial_events,
                  configuration_view const &) override;
  void configuration_update(std::size_t event_number,
                            configuration_view const &conf) override;

private:
  std::size_t m_next_target;
  std::size_t const m_target_step;
  std::size_t m_last_observed;
  std::size_t m_prev_target;
};
END_NAMESPACE_ADVANCE_ESTIMATORS
#endif