#include "generic_particle.h"

namespace simbad{
namespace core {

double GenericParticle::coord(size_t idx) const
{
    return ptr->coord(idx);
}

double GenericParticle::operator[](size_t idx) const
{
    assert( idx < dimension() );
    return ptr->coord( idx );
}

size_t GenericParticle::dimension() const
{
    return ptr->dimension();
}

size_t GenericParticle::particleType() const
{
    return ptr->particleType();
}

}}
