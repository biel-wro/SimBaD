#include "simple_exp_2d_factory.hpp"
#include "simple_exp_2d.hpp"

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
  return std::unique_ptr<simbad::core::model>(new simple_exp_2d);
}

std::string simple_exp_2d_factory::model_name() const
{
  return "simple_exp_2d";
}

std::size_t simple_exp_2d_factory::dimension() const { return 2; }

END_NAMESPACE_SIMPLE_EXP_2D
