#ifndef FRONT_WAVE_1D_FACTORY_HPP
#define FRONT_WAVE_1D_FACTORY_HPP

#include "front_wave_1d_fwd.hpp"

#include "model_factory.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D

class front_wave_1d_factory : public simbad::core::model_factory
{
public:
  front_wave_1d_factory();
  ~front_wave_1d_factory() override;

  std::unique_ptr<core::model> create_instance(const params &mp) const override;
  std::string model_name() const override;
  std::size_t dimension() const override;


};

END_NAMESPACE_FRONT_WAVE_1D

#endif
