#ifndef DISCRETE_WAVE_1D_HPP
#define DISCRETE_WAVE_1D_HPP

#include <memory>

#include "discrete_wave_1d_fwd.hpp"
#include "model.hpp"

BEGIN_NAMESPACE_DISCRETE_WAVE_1D

class discrete_wave_1d : public CORE_NAMESPACE::model
{
public:
  discrete_wave_1d(double alpha, double intensity_cap, std::size_t length,
                   double spacing, std::size_t seed);
  ~discrete_wave_1d();
  void generate_events(event_visitor visitor, size_t nevents) const override;
  std::size_t configuration_size() const override;
  void visit_configuration(particle_visitor v) const override;

protected:
  std::unique_ptr<discrete_wave_1d_impl> impl;
};

END_NAMESPACE_DISCRETE_WAVE_1D

#endif
