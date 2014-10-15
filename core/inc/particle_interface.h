#ifndef PARTICLE_INTERFACE_H
#define PARTICLE_INTERFACE_H

#include <cstddef>
#include <memory>
namespace simbad {
namespace core{

class ParticleInterface
{   
public:
    virtual ~ParticleInterface();
    virtual size_t dimension() const = 0;
    virtual double coord(size_t d) const = 0;
    virtual size_t particleType() const = 0;
    virtual std::unique_ptr<ParticleInterface> clone() const = 0;

};

}
}
#endif // ABSTRACTPARTICLE_H
