#ifndef CORE_FWD_HPP
#define CORE_FWD_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "computational/computational_fwd.hpp"
#include "configurations/stacked_view_configuration.hpp"
#include "interface/interface_fwd.hpp"
#include "processors/processors_fwd.hpp"


BEGIN_NAMESPACE_CORE
// containters
template <class, size_t> class LazySet;

// coordinates
template <class scalar_type_, std::size_t dimension_> struct coordinates;

// fixed

// legacy
class event_partial;
class simple_event_schedule;
class simple_event_queue;
template <class> class simple_event_view;
template <class> class simple_particle_view;
template <size_t, class> class simple_particle;
// object tracking



// other
template <class T> class object_factory;
template <class T> class class_register;
template <class Object>
Object *create_from_property_tree(property_tree const &);
END_NAMESPACE_CORE

#endif // CORE_FWD_HPP
