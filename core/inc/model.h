#ifndef CORE_MODEL_H
#define CORE_MODEL_H

#include <limits>

//#include "generic_particle.h"
//#include "virtual_iterator.h"

namespace simbad {
namespace core{


class Model
{
public:
    // types
    //typedef VirtualIterator<GenericParticle,GenericParticle> iterator;

    // constructors & destructors
    Model() noexcept;
    Model( Model &) = delete;
    Model( Model &&) = delete;
    Model &operator=(Model &) = delete;
    Model &operator=(Model &&) = delete;

    virtual ~Model() noexcept;

    // pure virtual
    virtual double run(std::size_t maxEvents, double maxTime=std::numeric_limits<double>::infinity() ) noexcept = 0;
    //virtual iterator begin() const noexcept = 0;
    //virtual iterator end() const noexcept = 0;

    virtual std::size_t particleCount() const noexcept = 0;
    virtual double simulationTime() const noexcept = 0;
    virtual std::size_t simulationEvents() const noexcept = 0;

    virtual std::string modelName() const noexcept;
    virtual int dimension() const noexcept;


};

}
}
#endif
