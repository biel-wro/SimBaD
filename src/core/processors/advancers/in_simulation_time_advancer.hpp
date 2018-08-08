
#ifndef SIMBAD_IN_SIMULATION_TIME_ADVANCER_HPP
#define SIMBAD_IN_SIMULATION_TIME_ADVANCER_HPP

#include "interface/dynamic_advancer.hpp"

BEGIN_NAMESPACE_CORE
class in_simulation_time_advancer final : public dynamic_advancer
{
public:
  explicit in_simulation_time_advancer(double time_step = 1.0,
                                       double start_time = 0.0);
  explicit in_simulation_time_advancer(property_tree const &pt);

  std::size_t estimate() const override;
  bool next_target() override;
  void set_description(attribute_description const &) override;
  void read_event(std::size_t , event const &) override;

private:
  double const m_time_step;
  double const m_start_time;

  std::size_t m_num_step;
  double m_target_time;

  std::size_t m_time_attribute_idx;
  // for dumb implementation only
  bool m_target_achieved;
};
END_NAMESPACE_CORE
#endif // SIMBAD_IN_SIMULATION_TIME_ADVANCER_HPP
