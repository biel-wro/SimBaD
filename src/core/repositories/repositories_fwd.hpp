#ifndef SIMBAD_CORE_REPOSITORIES_FWD_HPP
#define SIMBAD_CORE_REPOSITORIES_FWD_HPP

#include "core_def.hpp"
#include "interface/interface_fwd.hpp"

BEGIN_NAMESPACE_CORE

template <class Interface> struct factory;

template <class Interface> factory<Interface> make_global_repository();

#define SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECLARATION(INTERFACE)  \
  template <>                                                                  \
  simbad::core::factory<INTERFACE> make_global_repository<INTERFACE>()

// one specialization for each interface +instance in .cpp file
SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECLARATION(stream_printer);
SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECLARATION(advancer);

//#undef SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECLARATION

END_NAMESPACE_CORE
#endif // SIMBAD_CORE_REPOSITORIES_FWD_HPP