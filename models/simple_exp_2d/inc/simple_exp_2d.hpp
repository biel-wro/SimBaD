#ifndef SIMPLE_EXP_2D_HPP
#define SIMPLE_EXP_2D_HPP

#include "model.hpp"
#include "simple_exp_2d_def.hpp"

BEGIN_NAMESPACE_SIMPLE_EXP_2D
class simple_exp_2d : public simbad::core::model
{
public:
  simple_exp_2d();
  ~simple_exp_2d() override;

  void generate_events(event_visitor, size_t) const override;
  std::size_t configuration_size() const override;
  void visit_configuration(particle_visitor) const override;
};
END_NAMESPACE_SIMPLE_EXP_2D
#endif // SIMPLE_EXP_2D_HPP
