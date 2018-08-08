#ifndef SIMBAD_PROCESSORS_POPULATION_SIZE_ADVANCER_HPP
#define SIMBAD_PROCESSORS_POPULATION_SIZE_ADVANCER_HPP
#include "interface/static_advancer.hpp"

BEGIN_NAMESPACE_CORE
class population_size_advancer final : public static_advancer
{
public:
  explicit population_size_advancer(std::size_t initial, std::size_t step);
  explicit population_size_advancer(property_tree const &);

  std::size_t estimate() const override;
  bool next_target() override;

  void configuration_update(std::size_t event_number,
                            configuration_view const &conf) override;

private:
  std::size_t m_next_target;
  std::size_t m_target_step;

  bool m_target_achieved;
};
END_NAMESPACE_CORE
#endif // SIMBAD_POPULATION_SIZE_ADVANCER_HPP
