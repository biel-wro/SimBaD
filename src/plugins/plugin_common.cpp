#include <boost/preprocessor.hpp>

namespace simbad
{
namespace plugins
{
#define SIMBAD_MAKE_PLUGIN_INITIALIZER_DECLARATION(z, n, _)                    \
  namespace SIMBAD_PLUGIN##n { void initialize_plugin(); }

#define SIMBAD_INITIALIZE_PLUGIN(z, n, _)                                      \
  SIMBAD_PLUGIN##n::initialize_plugin();

BOOST_PP_REPEAT(SIMBAD_NPLUGINS, SIMBAD_MAKE_PLUGIN_INITIALIZER_DECLARATION, _)

void initialize_plugins()
{
  BOOST_PP_REPEAT(SIMBAD_NPLUGINS, SIMBAD_INITIALIZE_PLUGIN, _)
}
} // namespace plugins
} // namespace simbad
