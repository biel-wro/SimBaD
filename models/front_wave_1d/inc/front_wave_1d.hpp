#ifndef FRONT_WAVE_1D_HPP
#define FRONT_WAVE_1D_HPP

#include "front_wave_1d_fwd.hpp"

#include "model.hpp"
#include <memory>

BEGIN_NAMESPACE_FRONT_WAVE_1D

class front_wave_1d : public simbad::core::model
{
public:
  front_wave_1d(double alpha, double x0);
  void generate_events(event_visitor visitor, size_t nevents) const override;
  std::size_t configuration_size() const override;
  void visit_configuration(particle_visitor visitor) const override;

protected:
  std::unique_ptr<front_wave_1d_impl> impl;
};

END_NAMESPACE_FRONT_WAVE_1D
#endif // FRONT_WAVE_1D_HPP
