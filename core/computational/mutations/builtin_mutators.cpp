#include "builtin_mutators.hpp"
#include "mutator.hpp"
#include "utils/class_register.hpp"
#include "utils/object_factory.hpp"

#include "templ/skewed_beta_mutation.hpp"
#include "templ/skewed_polynomial_mutator.hpp"
#include "templ/stateless_beta_mutator.hpp"
#include "templ/uniform_step_mutator.hpp"

#include <memory>
#include <random>
#include <string>
BEGIN_NAMESPACE_CORE
namespace
{
#define SIMBAD_MAKE_VIRTUAL_WRAPPER(output_class, interface_class, input_type) \
  class output_class : public interface_class, public input_type               \
  {                                                                            \
  public:                                                                      \
    output_class(property_tree const &pt)                                      \
        : interface_class(pt), input_type(pt)                                  \
    {                                                                          \
    }                                                                          \
    ~output_class() {}                                                         \
    double operator()(double val, std::mt19937_64 &rng) const override         \
    {                                                                          \
      return input_type::operator()(val, rng);                                 \
    }                                                                          \
  };

#define SIMBAD_MAKE_MUTATOR_FACTORY(mutator_name, mutator_type)                \
  class mutator_name##_factory : public object_factory<mutator>                \
  {                                                                            \
    std::unique_ptr<mutator> create_instance(property_tree const &pt) const    \
    {                                                                          \
      mutator *ptr = new mutator_type(pt);                                     \
      return std::unique_ptr<mutator>(ptr);                                    \
    }                                                                          \
    std::string class_name() const { return #mutator_name; }                   \
  };

#define SIMBAD_MAKE_MUTATOR_FACTORY_MAKER(output_name, class_type)             \
  SIMBAD_MAKE_VIRTUAL_WRAPPER(output_name##_virt, mutator, class_type)         \
  SIMBAD_MAKE_MUTATOR_FACTORY(output_name, output_name##_virt)                 \
  std::unique_ptr<object_factory<mutator>> make_##output_name##_factory()      \
  {                                                                            \
    std::unique_ptr<object_factory<mutator>> ptr(new output_name##_factory);   \
    return ptr;                                                                \
  }

SIMBAD_MAKE_MUTATOR_FACTORY_MAKER(skewed_beta_mutator,
                                  skewed_beta_mutator<double>)
SIMBAD_MAKE_MUTATOR_FACTORY_MAKER(skewed_polynomial_mutator,
                                  skewed_polynomial_mutator<double>)
SIMBAD_MAKE_MUTATOR_FACTORY_MAKER(uniform_step_mutator,
                                  uniform_step_mutator<double>)
SIMBAD_MAKE_MUTATOR_FACTORY_MAKER(stateless_beta_mutator,
                                  stateless_beta_mutator<double>)

std::unique_ptr<class_register<mutator>> make_register()
{
  std::unique_ptr<class_register<mutator>> reg_ptr(new class_register<mutator>);

  reg_ptr->register_factory(make_skewed_beta_mutator_factory());
  reg_ptr->register_factory(make_skewed_polynomial_mutator_factory());
  reg_ptr->register_factory(make_uniform_step_mutator_factory());
  reg_ptr->register_factory(make_stateless_beta_mutator_factory());
  return reg_ptr;
}
}

const class_register<mutator> &get_builtin_mutators()
{
  static std::unique_ptr<class_register<mutator>> ptr = make_register();
  return *ptr;
}
END_NAMESPACE_CORE
