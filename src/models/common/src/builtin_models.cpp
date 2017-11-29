#include "builtin_models.hpp"
#include <boost/preprocessor.hpp>
#include <memory>

#include "interface/model_factory.hpp"
#include "interface/model_register.hpp"

#define SIMBAD_MAKE_MODEL_NAMESPACE(model_name) model_name

#define SIMBAD_MAKE_FACTORY_TYPENAME(model_name) model_name##_factory

#define SIMBAD_MAKE_INCLUDE_FNAME(model_name)                                  \
  model_name## / inc / ##SIMBAD_MAKE_FACTORY_TYPENAME(model_name).hpp

#define SIMBAD_MAKE_FACTORY_FULL_TYPE(model_name)                              \
  SIMBAD_MAKE_MODEL_NAMESPACE(model_name)::SIMBAD_MAKE_FACTORY_TYPENAME(       \
      model_name)

#define SIMBAD_MAKE_FACTORY_GETTER_DECLARATION(z, n, _)                        \
  namespace SIMBAD_MODEL##n                                                    \
  {                                                                            \
    std::unique_ptr<simbad::core::model_factory> get_factory();                \
  }

#define SIMBAD_REGISTER_FACTORY(z, n, _)                                       \
  reg.register_factory(                                                        \
      SIMBAD_MAKE_MODEL_NAMESPACE(SIMBAD_MODEL##n)::get_factory());

using namespace simbad::core;
using namespace simbad::models;

namespace simbad
{
namespace models
{
BOOST_PP_REPEAT(SIMBAD_NMODELS, SIMBAD_MAKE_FACTORY_GETTER_DECLARATION, _)

namespace common
{
const model_register &get_builtin_models()
{
  static std::unique_ptr<model_register> p_reg(nullptr);
  if(!p_reg)
  {
    p_reg.reset(new model_register);
    model_register &reg = *p_reg;
    BOOST_PP_REPEAT(SIMBAD_NMODELS, SIMBAD_REGISTER_FACTORY, _)
  }

  return *p_reg;
}
}
}
}
