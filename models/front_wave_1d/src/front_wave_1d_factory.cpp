#include "front_wave_1d_factory.hpp"

#include <memory>

#include "interface/property_tree.hpp"

#include "front_wave_1d.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D

front_wave_1d_factory::front_wave_1d_factory() {}

front_wave_1d_factory::~front_wave_1d_factory() {}

std::unique_ptr<core::model>
front_wave_1d_factory::create_instance(const params &mp) const
{
  double alpha = mp.get<double>("alpha");
  double x0 = mp.get<double>("x0", 1.0);

  front_wave_1d *p_model = new front_wave_1d(alpha, x0);

  return std::unique_ptr<core::model>(p_model);
}

std::string front_wave_1d_factory::model_name() const
{
  return "front_wave_1d";
}

std::size_t front_wave_1d_factory::dimension() const { return 1; }

std::unique_ptr<core::model_factory> get_factory()
{
  return std::unique_ptr<core::model_factory>(new front_wave_1d_factory);
}

END_NAMESPACE_FRONT_WAVE_1D
