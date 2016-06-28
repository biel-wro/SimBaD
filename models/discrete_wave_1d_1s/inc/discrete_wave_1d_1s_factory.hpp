#ifndef DISCRETE_WAVE_1D_1S_FACTORY_H
#define DISCRETE_WAVE_1D_1S_FACTORY_H
#include "discrete_wave_1d_1s_fwd.hpp"

#include "model_factory.hpp"

BEGIN_NAMESPACE_DISCRETE_WAVE_1D_1S
class discrete_wave_1d_1s_factory : public simbad::core::model_factory{
public:
  discrete_wave_1d_1s_factory();
  ~discrete_wave_1d_1s_factory();

  std::unique_ptr<core::model> create_instance(const params &mp) const override;
  std::string model_name() const override;
  std::size_t dimension() const override;
};
END_NAMESPACE_DISCRETE_WAVE_1D_1S

#endif // DISCRETE_WAVE_1D_1S_FACTORY_H
