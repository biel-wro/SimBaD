#ifndef SIMBAD_CORE_INTERFACE_FWD_HPP
#define SIMBAD_CORE_INTERFACE_FWD_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "core_def.hpp"

BEGIN_NAMESPACE_CORE

// non-enum types
class model_advancer;
class advance_estimator;
class attribute;
class attribute_description;
class attribute_descriptor;
class attribute_list;
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
class property_tree;
class static_advance_estimator;
class stream_printer;
class stream_reader;

// enums
enum class ATTRIBUTE_KIND : std::uint32_t;
enum class ATTRIBUTE_SCALAR : std::uint32_t;
enum class EVENT_KIND : std::uint32_t;

// template classes
template <class T> class array_attribute;
template <class T> class class_register;

// aliases
using event = attribute_list;

END_NAMESPACE_CORE
#endif