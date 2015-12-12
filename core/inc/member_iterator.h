#ifndef MEMBERITERATOR_H
#define MEMBERITERATOR_H

#include <iterator>
#include <memory>

#include "virtual_interface.h"

namespace simbad{
namespace core{

template< class BaseIterator, class Interface >
class MemberIterator: public Interface
{
public:
    typedef Interface interface;
    typedef typename interface::value_type value_type;
    typedef typename interface::reference reference;

    typedef MemberIterator<BaseIterator,interface> type;

    MemberIterator() = default;
    MemberIterator( BaseIterator const &bit): it(bit) {}
    MemberIterator( type const &) = default;
    MemberIterator( type &&) = default;

    type &operator=(type const & ) = default;
    type &operator=(type && ) = default;
    ~MemberIterator(){}

    interface *clone( ) const{
        return new type(*this);
    }

    void increment(){
        ++it;
    }

    virtual bool equal(const interface &o ) const {
        type const *bit = dynamic_cast<type const *>(&o);
        return bit!=nullptr && equal( *bit );
    }

    bool equal( type const &o) const{
        return it == o.it;
    }
    reference dereference(){
        return *it;
    }


private:
    BaseIterator it;

};

}
}

#endif // MEMBERITERATOR_H
