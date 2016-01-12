#ifndef PARTICLE_COORDS_HPP
#define PARTICLE_COORDS_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <initializer_list>

namespace simbad
{
namespace core
{

template <class coord_type, std::size_t DIM>
class particle_coords : public std::array<coord_type, DIM>
{
    using std::array<coord_type, DIM>::array;
};
}
}
#endif
