#ifndef CORE_FWD_HPP
#define CORE_FWD_HPP
#include <cstddef>
#include <cstdint>

namespace simbad
{
namespace core
{
enum class EVENT_KIND : uint32_t;

template<class, size_t> class LazySet;

class simple_event_schedule;
class simple_event_queue;

template <size_t, class>
class simple_particle;
}
}
#endif // CORE_FWD_HPP
