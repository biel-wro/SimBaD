
#ifndef SIMBAD_PROCESSORS_ADVANCE_ESTIMATORS_IN_SIMULATION_TIME_HPP
#define SIMBAD_PROCESSORS_ADVANCE_ESTIMATORS_IN_SIMULATION_TIME_HPP

#include "interface/dynamic_advance_estimator.hpp"

#include "advance_estimators_def.hpp"

BEGIN_NAMESPACE_ADVANCE_ESTIMATORS

class in_simulation_time final : public dynamic_advance_estimator
{
public:
  explicit in_simulation_time(double time_step = 1.0, double start_time = 0.0);
  explicit in_simulation_time(property_tree const &pt);

  std::size_t estimate() const override;
  bool next_target() override;
  void set_description(attribute_description const &) override;
  void event_update(std::size_t, event const &) override;

private:
  double const m_time_step;
  double const m_start_time;

  std::size_t m_num_step;
  double m_target_time;

  std::size_t m_time_attribute_idx;

  double m_last_observed_time;
};
END_NAMESPACE_ADVANCE_ESTIMATORS
#endif // SIMBAD_IN_SIMULATION_TIME_ADVANCER_HPP
