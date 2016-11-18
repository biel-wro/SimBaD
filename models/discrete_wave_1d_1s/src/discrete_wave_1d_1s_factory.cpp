#include "discrete_wave_1d_1s_factory.hpp"
#include "discrete_wave_1d_1s.hpp"

#include "property_tree.hpp"

BEGIN_NAMESPACE_DISCRETE_WAVE_1D_1S

discrete_wave_1d_1s_factory::discrete_wave_1d_1s_factory() {}

discrete_wave_1d_1s_factory::~discrete_wave_1d_1s_factory() {}

std::unique_ptr<core::model>
discrete_wave_1d_1s_factory::create_instance(const params &mp) const {
  double alpha = mp.get<double>("dispersion.alpha");
  double x0 = mp.get<double>("dispersion.x0");
  double k = mp.get<double>("k");
  std::size_t grid_length = mp.get<std::size_t>("grid.length");
  double spacing = mp.get<double>("grid.spacing");
  std::size_t seed = mp.get<std::size_t>("seed");

  return std::unique_ptr<core::model>(
      new discrete_wave_1d_1s(alpha, k, grid_length, x0, spacing, seed));
}

std::string discrete_wave_1d_1s_factory::model_name() const {
  return "discrete_wave_1d_1s";
}

std::size_t discrete_wave_1d_1s_factory::dimension() const { return 1; }

std::unique_ptr<core::model_factory> get_factory()
{
  return std::unique_ptr<core::model_factory>(new discrete_wave_1d_1s_factory);
}

END_NAMESPACE_DISCRETE_WAVE_1D_1S
