#ifndef SIMBAD_CORE_REPOSITORIES_FWD_HPP
#define SIMBAD_CORE_REPOSITORIES_FWD_HPP

#include "computational/computational_fwd.hpp"
#include "core_def.hpp"
#include "interface/interface_fwd.hpp"

BEGIN_NAMESPACE_CORE

template <class Interface> struct factory;

template <class Interface> factory<Interface> make_global_repository();

#define SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(INTERFACE)         \
  template <>                                                                  \
  simbad::core::factory<INTERFACE> make_global_repository<INTERFACE>()

// one specialization for each interface +instance in .cpp file
SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(advance_estimator);
SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(configuration_view);
SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(stream_printer);
SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(stream_reader);
SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL(time_dependent_scalar);

// undefine just in case
//#undef SIMBAD_MAKE_REPOSITORY_EXPLICIT_SPECIALIZATION_DECL

END_NAMESPACE_CORE
#endif // SIMBAD_CORE_REPOSITORIES_FWD_HPP
