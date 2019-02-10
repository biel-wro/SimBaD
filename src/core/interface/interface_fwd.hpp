#ifndef SIMBAD_CORE_INTERFACE_FWD_HPP
#define SIMBAD_CORE_INTERFACE_FWD_HPP

#include <type_traits>

#include "core_def.hpp"

#include "fwd/attribute_fwd.hpp"
#include "fwd/computational_fwd.hpp"
#include "fwd/io_fwd.hpp"
#include "fwd/properties_fwd.hpp"

BEGIN_NAMESPACE_CORE

// non-enum types
class model_advancer;
class advance_estimator;

class configuration_reader;
class configuration_view;
class dataframe;
class dynamic_advance_estimator;
class event_sink;
class event_source;
class finite_dataframe;
class model;
class model_factory;
class model_register;
class static_advance_estimator;

template <class> class class_register;
template <class> class extractor;

END_NAMESPACE_CORE
#endif