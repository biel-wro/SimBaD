#ifndef CORE_FWD_HPP
#define CORE_FWD_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "core_def.hpp"

BEGIN_NAMESPACE_CORE
// computational
class mutator;

// configurations

// containters
template <class, size_t> class LazySet;

// coordinates
template <class scalar_type_, std::size_t dimension_> struct coordinates;

// fixed

// interface
class attribute;
template <class T> class attribute_array;
class attribute_descriptor;
class attribute_descriptor_record;
class attribute_list;
class attribute_range;
enum class ATTRIBUTE_KIND : std::uint32_t;
class attribute_descriptor;
class configuration_reader;
class configuration_view;
class dataframe;
//class event;
using event = attribute_list;
class event_sink;
class event_source;
enum class EVENT_KIND : std::uint32_t;
class finite_dataframe;
class model;
class model_factory;
class model_register;
class object_descriptor;
class particle;
class property_tree;

// legacy
class event_partial;
class simple_event_schedule;
class simple_event_queue;
template <class> class simple_event_view;
template <class> class simple_particle_view;
template <size_t, class> class simple_particle;
// object tracking

// processors
class configuration_builder;
class snapshotter;
class stream_printer;

// utils
class argument_parser;
class counting_id_generator;
class generic_particle;

// other
template <class T> class object_factory;
template <class T> class class_register;
template <class Object>
Object *create_from_property_tree(property_tree const &);
END_NAMESPACE_CORE

#endif // CORE_FWD_HPP
