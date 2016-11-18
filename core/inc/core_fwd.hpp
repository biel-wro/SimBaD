#ifndef CORE_FWD_HPP
#define CORE_FWD_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "core_def.hpp"

BEGIN_NAMESPACE_CORE

class argument_parser;
class properties;

enum class EVENT_KIND : uint32_t;
class event;
class event_partial;

class object_descriptor;
class configuration_view;
class event_source;
class model;
class model_factory;

class model_loader;
class model_register;

class particle;
class property_tree;

template <class, size_t> class LazySet;

class simple_event_schedule;
class simple_event_queue;

template <class> class simple_event_view;

template <class> class simple_particle_view;

template <size_t, class> class simple_particle;

template <class Object>
Object *create_from_property_tree(property_tree const &);
END_NAMESPACE_CORE

#endif // CORE_FWD_HPP
