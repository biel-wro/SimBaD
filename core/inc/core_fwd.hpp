#ifndef CORE_FWD_HPP
#define CORE_FWD_HPP

#include <cstddef>
#include <cstdint>

#include "core_def.hpp"

BEGIN_NAMESPACE_CORE

enum class EVENT_KIND : uint32_t;
class event;
class event_partial;

class model;
class model_factory;

class model_loader;
class model_register;

class particle;

template <class, size_t>
class LazySet;

class simple_event_schedule;
class simple_event_queue;

template<class>
class simple_event_view;

template<class>
class simple_particle_view;

template <size_t, class>
class simple_particle;

END_NAMESPACE_CORE

#endif // CORE_FWD_HPP
