#ifndef BASEITERATOR_H
#define BASEITERATOR_H

namespace simbad{
namespace core{


template< class T, class ReferenceType >
class VirtualInterface{
public:
    typedef T value_type;
    typedef ReferenceType reference;

    virtual ~VirtualInterface();

    virtual VirtualInterface<T,ReferenceType> *clone() const = 0;

    virtual void increment() = 0;
    virtual bool equal( VirtualInterface const &o ) const = 0;
    virtual ReferenceType dereference() = 0;
};

}
}

#include "virtual_interface_impl.h"

#endif // BASEITERATOR_H
