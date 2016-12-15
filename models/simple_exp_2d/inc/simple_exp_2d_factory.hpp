#ifndef SIMPLE_EXP_2D_FACTORY_HPP
#define SIMPLE_EXP_2D_FACTORY_HPP

#include "simple_exp_2d_fwd.hpp"

#include "interface/model_factory.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D

class simple_exp_2d_factory : public simbad::core::model_factory
{
public:
  simple_exp_2d_factory();
  ~simple_exp_2d_factory() override;

  std::unique_ptr<core::model> create_instance(const params &mp) const override;
  std::string model_name() const override;
  std::size_t dimension() const override;
};

END_NAMESPACE_SIMPLE_EXP_2D

#endif
