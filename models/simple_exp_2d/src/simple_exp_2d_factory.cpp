#include "simple_exp_2d_factory.hpp"
#include "simple_exp_2d.hpp"

#include "interface/property_tree.hpp"
#include <random>

#include "dispersion.hpp"
#include "kernel.hpp"
#include "saturation_intensity.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D

std::unique_ptr<core::model_factory> get_factory()
{
  return std::unique_ptr<core::model_factory>(new simple_exp_2d_factory);
}

simple_exp_2d_factory::simple_exp_2d_factory() {}

simple_exp_2d_factory::~simple_exp_2d_factory() {}

std::unique_ptr<core::model> simple_exp_2d_factory::create_instance(
    const core::model_factory::params &mp) const
{

  dispersion d = dispersion::from_property_tree(mp.get_child("dispersion"));

  kernel k = kernel::from_property_tree(mp.get_child("interaction"));

  saturation_intensity s =
      saturation_intensity::from_property_tree(mp.get_child("saturation"));

  unsigned long long seed = mp.get<unsigned long long>("seed");
  std::mt19937_64 random_engine(seed);

  double death_rate = mp.get<double>("death.rate");
  std::exponential_distribution<> death_distribuiton(death_rate);

  double tile_size = mp.get<double>("space.tile.size");

  return std::unique_ptr<simbad::core::model>(
      new simple_exp_2d(d, k, s, random_engine, death_distribuiton, tile_size));
}

std::string simple_exp_2d_factory::model_name() const
{
  return "simple_exp_2d";
}

std::size_t simple_exp_2d_factory::dimension() const { return 2; }

END_NAMESPACE_SIMPLE_EXP_2D
