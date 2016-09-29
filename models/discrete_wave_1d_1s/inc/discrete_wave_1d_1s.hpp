#ifndef DISCRETE_WAVE_1D_HPP
#define DISCRETE_WAVE_1D_HPP

#include <memory>

#include "discrete_wave_1d_1s_fwd.hpp"
#include "model.hpp"

BEGIN_NAMESPACE_DISCRETE_WAVE_1D_1S

class discrete_wave_1d_1s : public CORE_NAMESPACE::model {
public:
  discrete_wave_1d_1s(double alpha, double intensity_cap, std::size_t length,
                      double x0, double spacing, std::size_t seed);
  ~discrete_wave_1d_1s();
  void generate_events(event_visitor visitor, size_t nevents) override;
  std::size_t configuration_size() const override;
  void visit_configuration(particle_visitor v) const override;

protected:
  std::unique_ptr<discrete_wave_1d_1s_impl> impl;
};

END_NAMESPACE_DISCRETE_WAVE_1D_1S

#endif
