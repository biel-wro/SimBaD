#ifndef CORE_MODEL_H
#define CORE_MODEL_H

#include <string>
#include <limits>

//#include "generic_particle.h"
//#include "virtual_iterator.h"

namespace simbad {
namespace core{


class model
{
public:
    // types
    //typedef VirtualIterator<GenericParticle,GenericParticle> iterator;
    struct const_iterator{};

    // constructors & destructors
    model() noexcept;
    model( model &) = delete;
    model( model &&) = delete;
    model &operator=(model &) = delete;
    model &operator=(model &&) = delete;

    virtual ~model() noexcept;

    // pure virtual
    virtual double run(std::size_t maxEvents, double maxTime=std::numeric_limits<double>::infinity() ) noexcept = 0;
    virtual const_iterator begin() const noexcept = 0;
    virtual const_iterator end() const noexcept = 0;

    virtual std::size_t particleCount() const noexcept = 0;
    virtual double simulationTime() const noexcept = 0;
    virtual std::size_t simulationEvents() const noexcept = 0;

    virtual std::string modelName() const noexcept = 0;
    virtual int dimension() const noexcept = 0;

};

}
}
#endif
