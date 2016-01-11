#include "particle_1d.hpp"

namespace simbad
{
namespace models
{

using namespace simbad::core;

particle_1D::particle_1D(float pos, simple_event_handle h)
    : simple_particle<1, float>({pos}, h)
{
}
}
}
