#ifndef SIMPLE_PARTICLE_HPP
#define SIMPLE_PARTICLE_HPP

#include "handle_trackee.hpp"
#include "ptr_updater.hpp"
#include "simple_event_queue.hpp"
#include "trackee.hpp"

#include <array>
#include <cstddef>

namespace simbad
{
namespace core
{

template <size_t DIM, class coord_type>
struct simple_particle : public handle_trackee<simple_event_handle,
                                               simple_particle<DIM, coord_type>>
{
    using handle_type = simple_event_handle;
    using coord_array_type = std::array<coord_type, DIM>;
    using my_base =
        handle_trackee<handle_type, simple_particle<DIM, coord_type>>;

    explicit simple_particle(handle_type h = handle_type(nullptr),
                             coord_array_type c = coord_array_type())
        : my_base(std::move(h)), coords(std::move(c))
    {
    }
    simple_particle(simple_particle const &) = delete;
    simple_particle(simple_particle &&o) = default;
    simple_particle &operator=(simple_particle const &) = delete;
    simple_particle &operator=(simple_particle &&o) = default;

    template <size_t cdim> void set_coordinate(coord_type c)
    {
        coords[cdim] = c;
    }
    template <size_t cdim> coord_type get_coordinate() const
    {
        return coords[cdim];
    }

    void set_handle(handle_type handle) { my_base::set_handle(handle); }
    handle_type get_handle() const { return my_base::get_handle(); }

  private:
    coord_array_type coords;
};
}
}

#endif
