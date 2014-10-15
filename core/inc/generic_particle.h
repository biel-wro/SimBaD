#ifndef CORE_GENERIC_PARTICLE_H
#define CORE_GENERIC_PARTICLE_H

#include <memory>

#include "particle_wrapper.h"

namespace simbad{
namespace core{

class GenericParticle
{
public:
    GenericParticle() = delete;
    GenericParticle( GenericParticle &) = delete;
    GenericParticle( GenericParticle && ) = default;

    template<class Base, class ...Args>
    explicit GenericParticle( Base const &b, Args const &...args) :
        ptr( new ParticleWrapper<Base,Args...>(b,args...) )
    {}

    GenericParticle &operator=(GenericParticle & ) = delete;
    GenericParticle &operator=(GenericParticle && o){
        std::swap(ptr, o.ptr);
        return *this;
    }

    double coord(size_t idx) const;
    double operator[](size_t idx) const;
    size_t dimension() const;
    size_t particleType() const;

private:
    std::unique_ptr<ParticleInterface> ptr;
};

}
}
#endif
