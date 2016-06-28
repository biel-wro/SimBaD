#ifndef DISCRETE_WAVE_1D_2S_HPP
#define DISCRETE_WAVE_1D_2S_HPP

#include <memory>

#include "discrete_wave_1d_2s_fwd.hpp"
#include "model.hpp"

BEGIN_NAMESPACE_DISCRETE_WAVE_1D_2S

class discrete_wave_1d_2s : public CORE_NAMESPACE::model {
public:
  discrete_wave_1d_2s(double alpha, double intensity_cap, std::size_t length,
                      double x0, double spacing, std::size_t seed);
  ~discrete_wave_1d_2s();
  void generate_events(event_visitor visitor, size_t nevents) const override;
  std::size_t configuration_size() const override;
  void visit_configuration(particle_visitor v) const override;

protected:
  std::unique_ptr<discrete_wave_1d_2s_impl> impl;
};

END_NAMESPACE_DISCRETE_WAVE_1D_2S

#endif
