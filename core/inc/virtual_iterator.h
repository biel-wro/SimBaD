#ifndef VIRTUALITERATOR_H
#define VIRTUALITERATOR_H

#include <boost/iterator/iterator_facade.hpp>

#include <memory>

#include "virtual_interface.h"
#include "member_iterator.h"

namespace simbad {
namespace core {


template<typename T, typename = void>
struct is_iterator
{
   static constexpr bool value = false;
};

template<typename T>
struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>::type>
{
   static constexpr bool value = true;
};

template <class Value,class ValueReference>
class VirtualIterator
  : public boost::iterator_facade<
        VirtualIterator<Value,ValueReference>,
        Value,
        boost::forward_traversal_tag,
        ValueReference
    >
{

 public:
    typedef VirtualIterator<Value,ValueReference> type;

    typedef Value value_type;
    typedef ValueReference reference;

    typedef VirtualInterface<value_type, reference> interface;



    VirtualIterator() = default;


    template< class BaseIterator >
    explicit VirtualIterator( BaseIterator const &baseItr )
        : pWrap( new MemberIterator<BaseIterator,interface>( baseItr ) )
    {
    }

    VirtualIterator( VirtualIterator const &o ):
        pWrap( o.pWrap->clone() )
    {}
    VirtualIterator(type &&) = default;

    ~VirtualIterator(){
    }

    VirtualIterator<Value,ValueReference> &operator=(VirtualIterator<Value,ValueReference> const &o) = default;
    VirtualIterator<Value,ValueReference> &operator=(VirtualIterator<Value,ValueReference> &&o) = default;

 private:
    friend class boost::iterator_core_access;
    template <class,class> friend class VirtualIterator;

    bool equal(VirtualIterator<Value,ValueReference> const& other) const
    {
        return pWrap->equal( *other.pWrap );
    }

    void increment()
    {
        pWrap->increment();
    }

    ValueReference dereference() const
    {
        return pWrap->dereference();
    }

    std::unique_ptr<interface> pWrap;
};

}
}

#endif // VIRTUALITERATOR_H
