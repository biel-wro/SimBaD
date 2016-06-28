#include "builtin_models.hpp"
#include <boost/preprocessor.hpp>
#include <memory>


#include "model_register.hpp"

#define SIMBAD_MAKE_MODEL_NAMESPACE(model_name) model_name

#define SIMBAD_MAKE_FACTORY_TYPENAME(model_name) model_name##_factory

#define SIMBAD_MAKE_INCLUDE_FNAME(model_name) model_name##/inc/##SIMBAD_MAKE_FACTORY_TYPENAME(model_name).hpp

#define SIMBAD_MAKE_FACTORY_FULL_TYPE(model_name)                              \
  SIMBAD_MAKE_MODEL_NAMESPACE(model_name) :: SIMBAD_MAKE_FACTORY_TYPENAME(model_name)

// model factories includes
#include "discrete_wave_1d_1s/inc/discrete_wave_1d_1s_factory.hpp"
#include "discrete_wave_1d_2s/inc/discrete_wave_1d_2s_factory.hpp"
#include "front_wave_1D/inc/front_wave_1d_factory.hpp"

using namespace simbad::core;
using namespace simbad::models;

namespace simbad {
namespace models {
namespace common {

template <class Factory> static void register_factory(model_register &reg) {
  std::unique_ptr<model_factory> factory(new Factory);
  reg.register_factory(std::move(factory));
}

#define SIMBAD_REGISTER_FACTORY(z, n, _ )                                       \
  register_factory<SIMBAD_MAKE_FACTORY_FULL_TYPE( SIMBAD_MODEL##n )>(reg);

const model_register &get_buiildin_models() {
  static std::unique_ptr<model_register> p_reg(nullptr);
  if (!p_reg) {
    p_reg.reset(new model_register);
    model_register &reg = *p_reg;
    BOOST_PP_REPEAT(SIMBAD_NMODELS, SIMBAD_REGISTER_FACTORY, _)
  }

  return *p_reg;
}

}
}
}
