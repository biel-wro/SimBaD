#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "core_def.hpp"

BEGIN_NAMESPACE_CORE

// non-enum types
class attribute;
class attribute_description;
class attribute_descriptor;
class attribute_list;
class attribute_range;
class configuration_reader;
class configuration_view;
class dataframe;
class event_sink;
class event_source;
class finite_dataframe;
class model;
class model_factory;
class model_register;
class object_descriptor;
class property_tree;

// enums
enum class ATTRIBUTE_KIND : std::uint32_t;
enum class ATTRIBUTE_SCALAR : std::uint32_t;
enum class EVENT_KIND : std::uint32_t;

// template classes
template <class T> class array_attribute;

// aliases
using event = attribute_list;

END_NAMESPACE_CORE